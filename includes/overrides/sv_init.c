/*
From SunVox docs:

SV_INIT_FLAG_NO_DEBUG_OUTPUT;
SV_INIT_FLAG_USER_AUDIO_CALLBACK - offline mode: system-dependent audio stream will not be created; user must call sv_audio_callback() to get the next piece of sound stream;
SV_INIT_FLAG_OFFLINE - same as SV_INIT_FLAG_USER_AUDIO_CALLBACK;
SV_INIT_FLAG_AUDIO_INT16 - desired sample type of the output sound stream : int16_t;
SV_INIT_FLAG_AUDIO_FLOAT32 - desired sample type of the output sound stream : float; the actual sample type may be different, if SV_INIT_FLAG_USER_AUDIO_CALLBACK is not set;
SV_INIT_FLAG_ONE_THREAD - audio callback and song modification are in single thread; use it with SV_INIT_FLAG_USER_AUDIO_CALLBACK only.
*/
static int get_init_flags(lua_State *L, int nparam) {
  int flags = 0;
  if (is_flag_set_true(L, nparam, "no_debug_output")) {
    flags |= SV_INIT_FLAG_NO_DEBUG_OUTPUT;
  }
  if (is_flag_set_true(L, nparam, "offline")) {
    flags |= SV_INIT_FLAG_OFFLINE;
  }
  // We don't cover "user_audio_callback" as it's the same as "offline"
  if (is_flag_set_true(L, nparam, "one_thread")) {
    flags |= SV_INIT_FLAG_ONE_THREAD;
  }

  int sample_type = get_int_param(L, nparam, "sample_type");
  if (sample_type == 16) {
    use_int16_t = 1;
    flags |= SV_INIT_FLAG_AUDIO_INT16;
  } else if (sample_type == 32) {
    use_int16_t = 0;
    flags |= SV_INIT_FLAG_AUDIO_FLOAT32;
  } else if (sample_type != 0) {
    luaL_error(L, "Parameter \"sample_type\" can be either 16 or 32, not %d\n", sample_type);
  }
  return flags;
}

static int lua_sv_init(lua_State *L) {
  const char* config = luaL_checkstring(L, 1);
  int freq = luaL_checkinteger(L, 2);
  int channels = luaL_checkinteger(L, 3);
  // Let's suppose we have params table instead of flags
  luaL_checktype(L, 4, LUA_TTABLE);

  uint32_t flags = get_init_flags(L, 4);

  // use_int16_t is defined on top of compiled file
  // see luavox.c when it's built
  if (flags & SV_INIT_FLAG_AUDIO_INT16) {
    use_int16_t = 1;
  } else if (flags & SV_INIT_FLAG_AUDIO_FLOAT32) {
    use_int16_t = 0;
  }

  int ret = sv_init(config, freq, channels, flags);
  lua_pushinteger(L, ret);
  return 1;
}
