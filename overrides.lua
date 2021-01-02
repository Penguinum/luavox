local overrides = {}

local function read_c_file(filename)
  local f = io.open("overrides/" .. filename .. ".c", "r")
  local content = f:read("*all")
  f:close()
  return content
end

overrides.sv_get_time_map = read_c_file("sv_get_time_map")
overrides.sv_audio_callback = read_c_file("sv_audio_callback")
overrides.sv_audio_callback2 = read_c_file("sv_audio_callback2")
overrides.sv_get_module_inputs = read_c_file("sv_get_module_inputs")
overrides.sv_get_module_outputs = read_c_file("sv_get_module_outputs")
overrides.sv_get_module_scope2 = read_c_file("sv_get_module_scope2")
overrides.sv_module_curve = read_c_file("sv_module_curve")

return overrides
