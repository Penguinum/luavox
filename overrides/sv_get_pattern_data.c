static int lua_sv_get_pattern_data(lua_State* L) {
  // sunvox_note* sv_get_pattern_data(int slot, int pat_num)
  int slot = luaL_checkinteger(L, 1);
  int pat_num = luaL_checkinteger(L, 2);

  int tracks = sv_get_pattern_tracks(slot, pat_num);
  int lines = sv_get_pattern_lines(slot, pat_num);
  int notes_num = tracks * lines;

  sunvox_note* notes = sv_get_pattern_data(slot, pat_num);
  lua_newtable(L);
  for (int i = 0; i < notes_num; i++) {
    lua_pushinteger(L, i + 1);
    Note *note = (Note *)lua_newuserdata(L, sizeof(Note));
    note->note = &notes[i];

    luaL_getmetatable(L, "Sunvox.note");
    lua_setmetatable(L, -2);
    lua_settable(L, -3);
  }

  return 1;
}
