static int lua_sv_get_module_outputs(lua_State* L) {
  int slot = (int)luaL_checknumber(L, 1);
  int mod_num = (int)luaL_checknumber(L, 2);

  uint32_t module_flags = sv_get_module_flags(slot, mod_num);

  int* ret = sv_get_module_outputs(slot, mod_num);

  size_t number_of_outputs = (module_flags & SV_MODULE_OUTPUTS_MASK) >> SV_MODULE_OUTPUTS_OFF;

  lua_newtable(L);
  for (int i = 0; i < number_of_outputs; i++) {
    lua_pushnumber(L, i + 1);
    lua_pushnumber(L, ret[i]);
    lua_settable(L, -3);
  }

  return 1;
}