static int lua_sv_get_module_scope2(lua_State* L) {
  //int slot, int mod_num, int channel, int16_t* dest_buf, uint32_t samples_to_read
  int slot = luaL_checkinteger(L, 1);
  int mod_num = luaL_checkinteger(L, 2);
  int channel = luaL_checkinteger(L, 3);
  uint32_t samples_to_read = check_unsigned_int(L, 5);
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
    lua_pushinteger(L, i + 1);
    lua_pushnumber(L, dest_buf[i]);
    lua_settable(L, 4);
  }

  free(dest_buf);

  lua_pushinteger(L, received);
  return 1;
}
