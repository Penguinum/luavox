typedef struct Note {
  sunvox_note note;
} Note;

// metatable method for handling "note[index]"
static int note_index (lua_State* L) {
  Note* note = luaL_checkudata(L, 1, "Sunvox.note");
  const char *index = luaL_checkstring(L, 2);
  if (strcmp(index, "note") == 0) {
    lua_pushinteger(L, note->note.note);
  } else if (strcmp(index, "vel") == 0) {
    lua_pushinteger(L, note->note.vel);
  } else if (strcmp(index, "module") == 0) {
    lua_pushinteger(L, note->note.module);
  } else if (strcmp(index, "ctl") == 0) {
    lua_pushinteger(L, note->note.ctl);
  } else if (strcmp(index, "ctl_val") == 0) {
    lua_pushinteger(L, note->note.ctl_val);
  } else {
    lua_pushnil(L);
  }
  return 1;
}

// metatable method for handle "note[index] = value"
static int note_newindex (lua_State* L) {
  Note* note = luaL_checkudata(L, 1, "Sunvox.note");
  const char *index = luaL_checkstring(L, 2);
  uint8_t value = (uint8_t)luaL_checkinteger(L, 3);
  if (strcmp(index, "note") == 0) {
    note->note.note = value;
  } else if (strcmp(index, "vel") == 0) {
    note->note.vel = value;
  } else if (strcmp(index, "module") == 0) {
    note->note.module = value;
  } else if (strcmp(index, "ctl") == 0) {
    note->note.ctl = value;
  } else if (strcmp(index, "ctl_val") == 0) {
    note->note.ctl_val = value;
  }
  return 0;
}

static const struct luaL_Reg SunvoxNote [] = {
  { "__index",  note_index },
  { "__newindex",  note_newindex },
  { NULL, NULL }
};

static int field_or_default(lua_State *L, const char *field, int default_value) {
  if (!lua_istable(L, 1)) {
    // stack: userdata
    return default_value;
  }
  // stack: table, userdata
  int value;
  lua_pushstring(L, field);
  // stack: table, userdata, key
  lua_gettable(L, 1);
  // stack: table, userdata, value
  if (lua_isnil(L, -1)) {
    value = default_value;
  } else {
    value = luaL_checkinteger(L, -1);
  }
  lua_pop(L, 1);
  // stack: table, userdata
  return value;
}

static int new_sunvox_note (lua_State *L) {
  Note *note = (Note *)lua_newuserdata(L, sizeof(Note));
  note->note.note = field_or_default(L, "note", 0);
  note->note.vel = field_or_default(L, "vel", 0);
  note->note.module = field_or_default(L, "module", 0);
  note->note.ctl = field_or_default(L, "ctl", 0);
  note->note.ctl_val = field_or_default(L, "ctl_val", 0);

  luaL_getmetatable(L, "Sunvox.note");
  lua_setmetatable(L, -2);

  return 1;
}
