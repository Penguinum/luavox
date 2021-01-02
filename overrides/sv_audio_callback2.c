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
