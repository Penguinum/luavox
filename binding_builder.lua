local overrides = require "overrides"
local ignores = require "ignores"

local LIB_TOP = [[
#include <dlfcn.h>
#define SUNVOX_MAIN
#include <sunvox.h>
#ifdef __cplusplus
#include "lua.hpp"
#else
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#endif


#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>

]]

local LIB_BOTTOM = [[


#ifdef __cplusplus
}
#endif
]]

local readers = {
  ["int"] = function(n)
    return ("(int)luaL_checknumber(L, %d)"):format(n)
  end,
  ["uint32_t"] = function(n)
    return ("(int)luaL_checknumber(L, %d)"):format(n)
  end,
  ["double"] = function(n)
    return ("(double)luaL_checknumber(L, %d)"):format(n)
  end,
  ["float"] = function(n)
    return ("(float)luaL_checknumber(L, %d)"):format(n)
  end,
  ["const char*"] = function(n)
    return ("luaL_checkstring(L, %d)"):format(n)
  end,
}

local writers = {
  ["int"] = function(var)
    return ("lua_pushnumber(L, %s)"):format(var)
  end,
  ["double"] = function(var)
    return ("lua_pushnumber(L, %s)"):format(var)
  end,
  ["float"] = function(var)
    return ("lua_pushnumber(L, %s)"):format(var)
  end,
  ["uint32_t"] = function(var)
    return ("lua_pushnumber(L, %s)"):format(var)
  end,
  ["const char*"] = function(var)
    return ("lua_pushstring(L, %s)"):format(var)
  end,
}

local function make_arg_name(name)
  return "lua_" .. name
end

local function make_function_call(fdef)
  local ret = ""
  if fdef.type ~= "void" then
    ret = fdef.type .. " ret = "
  end
  local args = {}
  for i = 1, #fdef.args do
    local arg = fdef.args[i]
    table.insert(args, make_arg_name(arg.name))
  end
  local args_as_string = table.concat(args, ", ")

  local call = ret .. fdef.name .. "(" .. args_as_string .. ");"

  return call
end

local function build_lua_function(c_function)
  if overrides[c_function.name] then
    return overrides[c_function.name]
  elseif ignores[c_function.name] then
    return nil
  end
  local function_name = "lua_" .. c_function.name

  local lines = {}
  table.insert(lines, "static int " .. function_name .. "(lua_State *L) {")

  if #c_function.args == 1 and c_function.args[1].type == "void" then
    c_function.args = {}
  end
  for i = 1, #c_function.args do
    local arg = c_function.args[i]
    local reader = readers[arg.type]
    if not reader then
      print("No reader for type " .. arg.type .. ", function " .. c_function.name)
      -- print(inspect(c_function))
      return nil
    end
    local line = "  " .. arg.type .. " " .. make_arg_name(arg.name) .. " = " .. reader(i) .. ";"
    table.insert(lines, line)
  end

  local fcall = "  " .. make_function_call(c_function)
  table.insert(lines, fcall)

  local writer = writers[c_function.type]
  if not writer then
    print("No writer for type " .. c_function.type .. ", function " .. c_function.name)
    return nil
  end

  local lua_ret = writer("ret")
  table.insert(lines, "  " .. lua_ret .. ";")

  if c_function.type ~= "void" then
    table.insert(lines, "  return 1;")
  else
    table.insert(lines, "  return 0;")
  end
  table.insert(lines, "}")
  return table.concat(lines, "\n")
end

local function build_functions_reg(functions, failures)
  local lines = {
    "static const struct luaL_Reg luavox [] = {"
  }

  for i = 1, #functions do
    local func = functions[i]
    if not failures[func.name] then
      table.insert(
        lines,
        "  { \"" .. func.name .. "\", lua_" .. func.name .. "},"
      )
    end
  end

  table.insert(
    lines,
    "  { NULL, NULL }"
  )
  table.insert(lines, "};")
  return table.concat(lines, "\n")
end

local LUAOPEN = [[


int luaopen_luavox(lua_State *L) {
  luaL_newlib(L, luavox);
  sv_load_dll();
  return 1;
}
]]

local function build_binding(functions)
  local compiled_functions = {}
  local failures = {}
  for i = 1, #functions do
    local func = functions[i]
    local compiled_function = build_lua_function(func)
    table.insert(compiled_functions, compiled_function)
    if not compiled_function then
      failures[func.name] = true
    end
  end
  return LIB_TOP .. table.concat(compiled_functions, "\n\n")
    .. "\n\n" .. build_functions_reg(functions, failures) .. LUAOPEN .. LIB_BOTTOM
end

return {
  build_binding = build_binding,
}
