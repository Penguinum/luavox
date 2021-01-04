static int lua_sv_audio_callback(lua_State* L) {
  int lua_frames = (int)luaL_checknumber(L, 2);
  int lua_latency = (int)luaL_checknumber(L, 3);
  uint32_t lua_out_time = (uint32_t)luaL_checknumber(L, 4);

  void *cbuf;

  if (use_int16_t) {
    printf("using int16_t\n");
    Buffer_int16_t *buf = luaL_checkudata(L, 1, "Sunvox.buffer_int16_t");
    cbuf = &(buf->values);
  } else {
    printf("using float\n");
    Buffer_float *buf = luaL_checkudata(L, 1, "Sunvox.buffer_float");
    cbuf = &(buf->values);
  }

  int lua_ret = sv_audio_callback(
    cbuf,
    lua_frames,
    lua_latency,
    lua_out_time
  );

  lua_pushnumber(L, lua_ret);
  return 1;
}
