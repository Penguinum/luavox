/*
From SunVox docs:

SV_TIME_MAP_SPEED: dest[X] = BPM | ( TPL << 16 ) (speed at the beginning of line X);
SV_TIME_MAP_FRAMECNT: dest[X] = frame counter at the beginning of line X.
*/
static int get_time_map_flags(lua_State *L, int nparam) {
  int flags = 0;

  const char* dest = get_string_param(L, nparam, "dest");
  if (strcmp(dest, "speed") == 0) {
    flags |= SV_TIME_MAP_SPEED;
  } else if (strcmp(dest, "framecnt") == 0) {
    flags |= SV_TIME_MAP_FRAMECNT;
  }
  return flags;
}

static int lua_sv_get_time_map(lua_State* L) {
    int lua_slot = luaL_checkinteger(L, 1);
    int lua_start_line = luaL_checkinteger(L, 2);
    int lua_len = luaL_checkinteger(L, 3);

    uint32_t* lua_dest = (uint32_t*)calloc(lua_len, sizeof(uint32_t));

    int lua_flags = get_time_map_flags(L, 4);

    int lua_ret = sv_get_time_map(
      lua_slot,
      lua_start_line,
      lua_len,
      lua_dest,
      lua_flags
    );
    lua_pushinteger(L, lua_ret);
    // lua stack: slot, line, len, flags, lua_ret
    lua_newtable(L); // time map
    for (int i = 0; i < lua_len; i++) {
      lua_pushinteger(L, i + 1);
      lua_pushinteger(L, lua_dest[i]);
      // lua stack: slot, line, len, flags, lua_ret, time_map, i, value
      lua_settable(L, -3);
    }
    free(lua_dest);
    // lua stack: slot, line, len, flags, lua_ret, time_map
    return 2;
  }
