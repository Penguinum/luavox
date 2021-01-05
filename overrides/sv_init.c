static int lua_sv_init(lua_State *L) {
  const char* config = luaL_checkstring(L, 1);
  int freq = luaL_checkinteger(L, 2);
  int channels = luaL_checkinteger(L, 3);
  uint32_t flags = check_unsigned_int(L, 4);

  // use_int16_t is defined on top of compiled file
  // see luavox.c when it's built
  if (flags & SV_INIT_FLAG_AUDIO_INT16) {
    use_int16_t = 1;
  } else {
    use_int16_t = 0;
  }

  int ret = sv_init(config, freq, channels, flags);
  lua_pushinteger(L, ret);
  return 1;
}
