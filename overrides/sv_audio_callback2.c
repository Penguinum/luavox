static int lua_sv_audio_callback2(lua_State* L) {
    int lua_frames = (int)luaL_checknumber(L, 2);
    int lua_latency = (int)luaL_checknumber(L, 3);
    uint32_t lua_out_time = (uint32_t)luaL_checknumber(L, 4);
    int lua_in_type = (int)luaL_checknumber(L, 5);
    int lua_in_channels = (int)luaL_checknumber(L, 6);

    void *c_buf, *c_in_buf;

    if (use_int16_t) {
      Buffer_int16_t *buf = luaL_checkudata(L, 1, "Sunvox.buffer_int16_t");
      Buffer_int16_t *in_buf = luaL_checkudata(L, 1, "Sunvox.buffer_int16_t");
      c_buf = &(buf->values);
      c_in_buf = &(buf->values);
    } else {
      Buffer_float *buf = luaL_checkudata(L, 1, "Sunvox.buffer_float");
      Buffer_float *in_buf = luaL_checkudata(L, 1, "Sunvox.buffer_float");
      c_buf = &(buf->values);
      c_in_buf = &(buf->values);
    }

    int lua_ret = sv_audio_callback2(
      c_buf,
      lua_frames,
      lua_latency,
      lua_out_time,
      lua_in_type,
      lua_in_channels,
      c_in_buf
    );

    lua_pushnumber(L, lua_ret);
    return 1;
  }
