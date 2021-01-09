local structure_bindings = require "structure_bindings"
local function_bindings = require "function_bindings"

local LIB_TOP = [[
#ifdef __unix__
#include <dlfcn.h>
#endif

#include <string.h>
#define SUNVOX_MAIN
#include <sunvox.h>
#ifdef __cplusplus
#include "lua.hpp"
#else
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#endif

static int use_int16_t = 1;

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>

static uint32_t check_unsigned_int(lua_State *L, int arg) {
  int ret = luaL_checkinteger(L, arg);
  if (ret < 0) {
    return luaL_error(L, "bad argument #%d, expected unsigned integer", arg);
  }
  return (uint32_t)ret;
}

]]

local LIB_BOTTOM = [[


#ifdef __cplusplus
}
#endif
]]


local LUAOPEN = [[


int luaopen_luavox(lua_State *L) {
  luaL_newmetatable(L, "Sunvox.note");
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, SunvoxNote, 0);

  luaL_newmetatable(L, "Sunvox.buffer_int16_t");
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, SunvoxBuffer_int16_t, 0);

  luaL_newmetatable(L, "Sunvox.buffer_float");
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, SunvoxBuffer_float, 0);

  luaL_newlib(L, luavox);
  sv_load_dll();
  return 1;
}
]]

local function build_binding(functions)
  local built_data_structures = structure_bindings.build()
  local built_functions = function_bindings.build(functions)
  return LIB_TOP
    .. built_data_structures
    .. built_functions
    .. LUAOPEN
    .. LIB_BOTTOM
end

return {
  build_binding = build_binding,
}
