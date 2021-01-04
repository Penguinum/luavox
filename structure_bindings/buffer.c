typedef struct Buffer_SV_NUM {
  size_t size;
  SV_NUM values[1];
} Buffer_SV_NUM;

// metatable method for handling "buffer[index]"
static int buffer_SV_NUM_index(lua_State* L) {
  Buffer_SV_NUM* buf = luaL_checkudata(L, 1, "Sunvox.buffer_SV_NUM");
  int index = luaL_checkinteger(L, 2);
  if (index < 1 || index > buf->size) {
    return luaL_error(L, "Sunvox buffer access error: index should be between 1 and %d\n", buf->size);
  }
  lua_pushnumber(L, buf->values[index - 1]);
  return 1;
}

// metatable method for handle "buffer[index] = value"
static int buffer_SV_NUM_newindex(lua_State *L) {
  Buffer_SV_NUM* buf = luaL_checkudata(L, 1, "Sunvox.buffer_SV_NUM");
  int index = luaL_checkinteger(L, 2);
  if (index < 1 || index > buf->size) {
    return luaL_error(L, "Sunvox buffer access error: index should be between 1 and %d\n", buf->size);
  }
  SV_NUM value = luaL_checknumber(L, 3);
  buf->values[index - 1] = value;
  return 0;
}

static int buffer_SV_NUM_len(lua_State *L) {
  Buffer_SV_NUM* buf = luaL_checkudata(L, 1, "Sunvox.buffer_SV_NUM");
  lua_pushinteger(L, buf->size);
  return 1;
}

static const struct luaL_Reg SunvoxBuffer_SV_NUM [] = {
  { "__index",  buffer_SV_NUM_index },
  { "__newindex",  buffer_SV_NUM_newindex },
  { "__len",  buffer_SV_NUM_len },
  { NULL, NULL }
};

static int new_buffer_SV_NUM(lua_State *L) {
  int n = luaL_checkinteger(L, 1);
  if (n < 0) {
    return luaL_error(L, "Bad buffer size: %d\n", n);
  }
  size_t nbytes = sizeof(Buffer_SV_NUM) + (n - 1) * sizeof(SV_NUM);
  Buffer_SV_NUM *buf = (Buffer_SV_NUM *)lua_newuserdata(L, nbytes);
  buf->size = n;
  memset(buf->values, 0, sizeof(buf->values));

  luaL_getmetatable(L, "Sunvox.buffer_SV_NUM");
  lua_setmetatable(L, -2);
  return 1;
}
