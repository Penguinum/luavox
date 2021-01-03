#ifdef SV_INIT_FLAG_AUDIO_INT16
  #define SV_NUM int16_t
#else
  #define SV_NUM float
#endif

typedef struct Buffer {
  size_t size;
  SV_NUM values[1];
} Buffer;

int check_can_access(lua_State *L, Buffer *buf, int index) {
  if (index < 1 || index > buf->size) {
    return luaL_error(L, "Sunvox buffer access error: index should be between 1 and %d\n", buf->size);
  }
  return 0;
}

// metatable method for handling "buffer[index]"
static int buffer_index(lua_State* L) {
  Buffer* buf = luaL_checkudata(L, 1, "Sunvox.buffer");
  int index = luaL_checkinteger(L, 2);
  if (index < 1 || index > buf->size) {
    return luaL_error(L, "Sunvox buffer access error: index should be between 1 and %d\n", buf->size);
  }
  lua_pushnumber(L, buf->values[index - 1]);
  return 1;
}

// metatable method for handle "buffer[index] = value"
static int buffer_newindex (lua_State *L) {
  Buffer* buf = luaL_checkudata(L, 1, "Sunvox.buffer");
  int index = luaL_checkinteger(L, 2);
  if (index < 1 || index > buf->size) {
    return luaL_error(L, "Sunvox buffer access error: index should be between 1 and %d\n", buf->size);
  }
  SV_NUM value = luaL_checknumber(L, 3);
  buf->values[index - 1] = value;
  return 0;
}

static int buffer_len(lua_State *L) {
  Buffer* buf = luaL_checkudata(L, 1, "Sunvox.buffer");
  lua_pushinteger(L, buf->size);
  return 1;
}

static const struct luaL_Reg SunvoxBuffer [] = {
  { "__index",  buffer_index },
  { "__newindex",  buffer_newindex },
  { "__len",  buffer_len },
  { NULL, NULL }
};

static int new_buffer (lua_State *L) {
  int n = luaL_checkinteger(L, 1);
  if (n < 0) {
    return luaL_error(L, "Bad buffer size: %d\n", n);
  }
  size_t nbytes = sizeof(Buffer) + (n - 1) * sizeof(SV_NUM);
  Buffer *buf = (Buffer *)lua_newuserdata(L, nbytes);
  buf->size = n;
  memset(buf->values, 0, sizeof(buf->values));

  luaL_getmetatable(L, "Sunvox.buffer");
  lua_setmetatable(L, -2);
  return 1;
}
