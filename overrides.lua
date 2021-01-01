local overrides = {}

overrides.sv_get_time_map = [[
static int lua_sv_get_time_map(lua_State* L) {
  int lua_slot = (int)luaL_checknumber(L, 1);
  int lua_start_line = (int)luaL_checknumber(L, 2);
  int lua_len = (int)luaL_checknumber(L, 3);

  uint32_t* lua_dest = (uint32_t*)calloc(lua_len, sizeof(uint32_t));

  int lua_flags = (int)luaL_checknumber(L, 5);

  int lua_ret = sv_get_time_map(
    lua_slot,
    lua_start_line,
    lua_len,
    lua_dest,
    lua_flags
  );
  // lua stack: slot, line, len, {}, flags
  for (int i = 0; i < lua_len; i++) {
    lua_pushnumber(L, i + 1);
    lua_pushnumber(L, lua_dest[i]);
    // lua stack: slot, line, len, {}, flags, i, value
    lua_settable(L, -4);
  }
  lua_pushnumber(L, lua_ret);
  free(lua_dest);

  return 1;
}
]]

overrides.sv_audio_callback = [[
static int lua_sv_audio_callback(lua_State* L) {

  int lua_frames = (int)luaL_checknumber(L, 2);
  int lua_latency = (int)luaL_checknumber(L, 3);
  uint32_t lua_out_time = (uint32_t)luaL_checknumber(L, 4);

  #ifdef SV_INIT_FLAG_AUDIO_FLOAT32
    #define NUM float
  #else
    #define NUM int16_t
  #endif
  NUM* lua_buf = (NUM*)calloc(lua_frames, sizeof(NUM));
  // lua stack: {}, frames, latency, out_time

  int lua_ret = sv_audio_callback(
    lua_buf,
    lua_frames,
    lua_latency,
    lua_out_time
  );

  for (int i = 0; i < lua_frames; i++) {
    lua_pushnumber(L, i + 1);
    lua_pushnumber(L, lua_buf[i]);
    lua_settable(L, 1);
  }

  lua_pushnumber(L, lua_ret);
  free(lua_buf);

  return 1;
}
]]

overrides.sv_audio_callback2 = [[
static int lua_sv_audio_callback2(lua_State* L) {

  int lua_frames = (int)luaL_checknumber(L, 2);
  int lua_latency = (int)luaL_checknumber(L, 3);
  uint32_t lua_out_time = (uint32_t)luaL_checknumber(L, 4);
  int lua_in_type = (int)luaL_checknumber(L, 5);
  int lua_in_channels = (int)luaL_checknumber(L, 6);

  #ifdef SV_INIT_FLAG_AUDIO_FLOAT32
    #define NUM float
  #else
    #define NUM int16_t
  #endif
  NUM* lua_buf = (NUM*)calloc(lua_frames, sizeof(NUM));
  NUM* lua_in_buf = (NUM*)calloc(lua_frames, sizeof(NUM));
  // lua stack: {}, frames, latency, out_time, in_type, in_channels, in_buf

  for (int i = 0; i < lua_frames; i++) {
    lua_pushnumber(L, i);
    lua_gettable(L, -2);
    lua_in_buf[i] = (NUM)luaL_checknumber(L, -1);
    lua_pop(L, -1);
  }

  int lua_ret = sv_audio_callback2(
    lua_buf,
    lua_frames,
    lua_latency,
    lua_out_time,
    lua_in_type,
    lua_in_channels,
    lua_in_buf
  );

  for (int i = 0; i < lua_frames; i++) {
    lua_pushnumber(L, i + 1);
    lua_pushnumber(L, lua_buf[i]);
    lua_settable(L, 1);
  }

  lua_pushnumber(L, lua_ret);
  free(lua_buf);

  return 1;
}
]]

overrides.sv_get_module_inputs = [[
static int lua_sv_get_module_inputs(lua_State* L) {
  int slot = (int)luaL_checknumber(L, 1);
  int mod_num = (int)luaL_checknumber(L, 2);

  uint32_t module_flags = sv_get_module_flags(slot, mod_num);

  int* ret = sv_get_module_inputs(slot, mod_num);

  size_t number_of_inputs = (module_flags & SV_MODULE_INPUTS_MASK) >> SV_MODULE_INPUTS_OFF;

  lua_newtable(L);
  for (int i = 0; i < number_of_inputs; i++) {
    lua_pushnumber(L, i + 1);
    lua_pushnumber(L, ret[i]);
    lua_settable(L, -3);
  }

  return 1;
}
]]

overrides.sv_get_module_outputs = [[
static int lua_sv_get_module_outputs(lua_State* L) {
  int slot = (int)luaL_checknumber(L, 1);
  int mod_num = (int)luaL_checknumber(L, 2);

  uint32_t module_flags = sv_get_module_flags(slot, mod_num);

  int* ret = sv_get_module_outputs(slot, mod_num);

  size_t number_of_outputs = (module_flags & SV_MODULE_OUTPUTS_MASK) >> SV_MODULE_OUTPUTS_OFF;

  lua_newtable(L);
  for (int i = 0; i < number_of_outputs; i++) {
    lua_pushnumber(L, i + 1);
    lua_pushnumber(L, ret[i]);
    lua_settable(L, -3);
  }

  return 1;
}
]]

return overrides
