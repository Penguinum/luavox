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
