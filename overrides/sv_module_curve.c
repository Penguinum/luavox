static int lua_sv_module_curve(lua_State *L) {
  // int slot, int mod_num, int curve_num, float* data, int len, int w
  int slot = luaL_checkinteger(L, 1);
  int mod_num = luaL_checkinteger(L, 2);
  int curve_num = luaL_checkinteger(L, 3);
  int len = luaL_checkinteger(L, 5);
  int w = luaL_checkinteger(L, 6);

  float *data = (float*)calloc(len, sizeof(float));
  int ret = sv_module_curve(slot,
    mod_num,
    curve_num,
    data,
    len,
    w
  );

  for (int i = 0; i < len; i++) {
    lua_pushinteger(L, i + 1);
    lua_pushnumber(L, data[i]);
    lua_settable(L, 4);
  }
  free(data);
  lua_pushinteger(L, ret);
  return 1;
}
