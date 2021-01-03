local overrides = {}

local function register_override(func_name)
  local f = io.open("overrides/" .. func_name .. ".c", "r")
  local file_content = f:read("*all")
  f:close()
  overrides[func_name] = file_content
end

register_override("sv_get_time_map")
register_override("sv_audio_callback")
register_override("sv_audio_callback2")
register_override("sv_get_module_inputs")
register_override("sv_get_module_outputs")
register_override("sv_get_module_scope2")
register_override("sv_module_curve")
register_override("sv_get_pattern_data")

return overrides
