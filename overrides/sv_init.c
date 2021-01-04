static int lua_sv_init(lua_State *L) {
  const char* config = luaL_checkstring(L, 1);
  int freq = (int)luaL_checknumber(L, 2);
  int channels = (int)luaL_checknumber(L, 3);
  uint32_t flags = (int)luaL_checknumber(L, 4);

  // use_int16_t is defined on top of compiled file
  // see luavox.c when it's built
  if (flags & SV_INIT_FLAG_AUDIO_INT16) {
    use_int16_t = 1;
  } else {
    use_int16_t = 0;
  }

  int ret = sv_init(config, freq, channels, flags);
  lua_pushnumber(L, ret);
  return 1;
}
