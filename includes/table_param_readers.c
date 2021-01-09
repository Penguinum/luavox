static int is_flag_set_true(lua_State *L, int nparam, const char *key) {
  lua_pushstring(L, key);
  lua_gettable(L, nparam);
  int ltype = lua_type(L, -1);
  if (ltype == LUA_TNIL) {
    return 0;
  } else if (ltype == LUA_TBOOLEAN) {
    return lua_toboolean(L, -1);
  } else {
    return 1;
  }
}

static int get_int_param(lua_State *L, int nparam, const char *key) {
  lua_pushstring(L, key);
  lua_gettable(L, nparam);
  int ltype = lua_type(L, -1);
  if (ltype == LUA_TNIL) {
    return 0;
  } else if (ltype == LUA_TNUMBER) {
    return luaL_checkinteger(L, -1);
  } else {
    luaL_error(L, "Bad parameter type for '%s': expected number, got %s\n", key, lua_typename(L, ltype));
    return 0; // won't get there tbh
  }
}
