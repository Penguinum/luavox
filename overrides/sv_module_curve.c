static int lua_sv_module_curve(lua_State *L) {
  // int slot, int mod_num, int curve_num, float* data, int len, int w
  int slot = (int)luaL_checknumber(L, 1);
  int mod_num = (int)luaL_checknumber(L, 2);
  int curve_num = (int)luaL_checknumber(L, 3);
  int len = (int)luaL_checknumber(L, 5);
  int w = (int)luaL_checknumber(L, 6);

  float *data = (float*)calloc(len, sizeof(float));
  int ret = sv_module_curve(slot,
    mod_num,
    curve_num,
    data,
    len,
    w
  );

  for (int i = 0; i < len; i++) {
    lua_pushnumber(L, i + 1);
    lua_pushnumber(L, data[i]);
    lua_settable(L, 4);
  }
  free(data);
  lua_pushnumber(L, ret);
  return 1;
}
