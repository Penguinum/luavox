static int lua_sv_audio_callback(lua_State* L) {
  int lua_frames = luaL_checkinteger(L, 2);
  int lua_latency = luaL_checkinteger(L, 3);
  uint32_t lua_out_time = check_unsigned_int(L, 4);

  void *cbuf;

  if (use_int16_t) {
    Buffer_int16_t *buf = luaL_checkudata(L, 1, "Sunvox.buffer_int16_t");
    cbuf = &(buf->values);
  } else {
    Buffer_float *buf = luaL_checkudata(L, 1, "Sunvox.buffer_float");
    cbuf = &(buf->values);
  }

  int lua_ret = sv_audio_callback(
    cbuf,
    lua_frames,
    lua_latency,
    lua_out_time
  );

  lua_pushinteger(L, lua_ret);
  return 1;
}
