local function read_file(name)
  local fd = io.open(name, "r")
  local content = fd:read("*all")
  fd:close()
  return content
end

local function build()
  local code_parts = {}

  table.insert(code_parts, read_file("structure_bindings/sunvox_note.c"))
  local buffer_common = read_file("structure_bindings/buffer.c")
  local buffer_float = buffer_common:gsub("SV_NUM", "float")
  local buffer_int16_t = buffer_common:gsub("SV_NUM", "int16_t")

  table.insert(code_parts, buffer_float)
  table.insert(code_parts, buffer_int16_t)
  table.insert(code_parts, read_file("structure_bindings/buffer_constructor.c"))

  return table.concat(code_parts, "\n\n")
end

return {
  build = build
}
