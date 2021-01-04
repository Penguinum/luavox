static int new_buffer(lua_State *L) {
  if (use_int16_t) {
    return new_buffer_int16_t(L);
  } else {
    return new_buffer_float(L);
  }
}
