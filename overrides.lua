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
  float* lua_buf = (float*)calloc(lua_frames, sizeof(float));
  #else
  int16_t* lua_buf = (int16_t*)calloc(frames, sizeof(int16_t));
  #endif
  // lua stack: {}, frames, latency, out_time

  int lua_ret = sv_audio_callback(lua_buf, lua_frames, lua_latency, lua_out_time);

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

return overrides
