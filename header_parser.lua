local function split_type_and_name(arg)
  local split_pattern = "[ ]*(.+) ([%w_]+)"
  local atype, aname = arg:match(split_pattern)

  if not atype then -- fallback for void?
    if arg:match("void") then
      atype = "void"
    end
  end

  return atype, aname
end

local function extract_argument_list_from_string(arg_string)
  local arg_list = {}
  for _str in arg_string:gmatch("([^,]+)") do
    local atype, aname = split_type_and_name(_str)
    table.insert(arg_list, { type = atype, name = aname })
  end
  return arg_list
end

local function parse_line(fstring)
  local function_pattern = [[typedef (.-) %(SUNVOX_FN_ATTR *(.-)%)%( (.-) %);]]
  local ftype, fname, args_as_str = fstring:match(function_pattern)

  if not ftype or not fname or not args_as_str then
    return nil
  end

  if fname:sub(1, 2) == "*t" then
    fname = fname:sub(3)
  end

  local arg_list = extract_argument_list_from_string(args_as_str)

  return {
    type = ftype, name = fname, args = arg_list
  }
end

local function parse(header)
  local functions = parse_line(header)

  for str in header:gmatch("[^\n]+") do
    table.insert(functions, parse_line(str))
  end

  return functions
end

return {
  parse_line = parse_line,
  parse = parse,
}
