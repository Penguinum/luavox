static int lua_sv_get_module_scope2(lua_State* L) {
  //int slot, int mod_num, int channel, int16_t* dest_buf, uint32_t samples_to_read
  int slot = (int)luaL_checknumber(L, 1);
  int mod_num = (int)luaL_checknumber(L, 2);
  int channel = (int)luaL_checknumber(L, 3);
  uint32_t samples_to_read = (uint32_t)luaL_checknumber(L, 5);
  int16_t* dest_buf = (int16_t*)calloc(samples_to_read, sizeof(int16_t));

  uint32_t received = sv_get_module_scope2(
    slot,
    mod_num,
    channel,
    dest_buf,
    samples_to_read
  );

  for (int i = 0; i < received; i++) {
    int16_t sample = dest_buf[i];
    // table is in 4th position of stack
    lua_pushnumber(L, i + 1);
    lua_pushnumber(L, dest_buf[i]);
    lua_settable(L, 4);
  }

  free(dest_buf);

  lua_pushnumber(L, received);
  return 1;
}