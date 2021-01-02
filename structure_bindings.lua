local function read_file(name)
  local fd = io.open(name, "r")
  local content = fd:read("*all")
  fd:close()
  return content
end

local function build()
  local code_parts = {}

  table.insert(code_parts, read_file("structure_bindings/sunvox_note.c"))

  return table.concat(code_parts, "\n\n")
end

return {
  build = build
}
