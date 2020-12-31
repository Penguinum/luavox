#!/usr/bin/env lua

local header_parser = require("header_parser")
local binding_builder = require("binding_builder")

local function exit_with_message(msg, omit_usage)
  print(msg)
  if not omit_usage then
    print("Usage: " .. arg[-1] .. " " .. arg[0]
      .. " path/to/sunvox.h lang binding.c"
    )
  end
  os.exit(1)
end

local function main()
  local header_path = arg[1]
  local out_file = arg[2]

  if not header_path then
    exit_with_message("Please provide valid path to sunvox header")
  end

  if not out_file then
    exit_with_message("Please provide valid path to output file")
  end

  local fd, err = io.open(header_path, "r")
  if not fd then
    exit_with_message(err)
  end
  local header = fd:read("*all")
  fd:close()

  local functions = header_parser.parse(header)
  local code = binding_builder.build_binding(functions)

  fd, err = io.open(out_file, "w")
  if not fd then
    exit_with_message(err)
  end

  fd:write(code)
  fd:close()

  print("Successfully built sunvox binding")
end

main()
