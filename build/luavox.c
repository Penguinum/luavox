#ifdef __unix__
#include <dlfcn.h>
#endif

#include <string.h>
#define SUNVOX_MAIN
#include <sunvox.h>
#ifdef __cplusplus
#include "lua.hpp"
#else
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#endif

static int use_int16_t = 1;

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>

static uint32_t check_unsigned_int(lua_State *L, int arg) {
  int ret = luaL_checkinteger(L, arg);
  if (ret < 0) {
    return luaL_error(L, "bad argument #%d, expected unsigned integer", arg);
  }
  return (uint32_t)ret;
}

typedef struct Note {
  sunvox_note *note;
} Note;

// metatable method for handling "note[index]"
static int note_index (lua_State* L) {
  Note* note = luaL_checkudata(L, 1, "Sunvox.note");
  const char *index = luaL_checkstring(L, 2);
  if (strcmp(index, "note") == 0) {
    lua_pushinteger(L, note->note->note);
  } else if (strcmp(index, "vel") == 0) {
    lua_pushinteger(L, note->note->vel);
  } else if (strcmp(index, "module") == 0) {
    lua_pushinteger(L, note->note->module);
  } else if (strcmp(index, "ctl") == 0) {
    lua_pushinteger(L, note->note->ctl);
  } else if (strcmp(index, "ctl_val") == 0) {
    lua_pushinteger(L, note->note->ctl_val);
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
    note->note->note = value;
  } else if (strcmp(index, "vel") == 0) {
    note->note->vel = value;
  } else if (strcmp(index, "module") == 0) {
    note->note->module = value;
  } else if (strcmp(index, "ctl") == 0) {
    note->note->ctl = value;
  } else if (strcmp(index, "ctl_val") == 0) {
    note->note->ctl_val = value;
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


typedef struct Buffer_float {
  size_t size;
  float values[1];
} Buffer_float;

// metatable method for handling "buffer[index]"
static int buffer_float_index(lua_State* L) {
  Buffer_float* buf = luaL_checkudata(L, 1, "Sunvox.buffer_float");
  int index = luaL_checkinteger(L, 2);
  if (index < 1 || index > buf->size) {
    return luaL_error(L, "Sunvox buffer access error: index should be between 1 and %d\n", buf->size);
  }
  lua_pushnumber(L, buf->values[index - 1]);
  return 1;
}

// metatable method for handle "buffer[index] = value"
static int buffer_float_newindex(lua_State *L) {
  Buffer_float* buf = luaL_checkudata(L, 1, "Sunvox.buffer_float");
  int index = luaL_checkinteger(L, 2);
  if (index < 1 || index > buf->size) {
    return luaL_error(L, "Sunvox buffer access error: index should be between 1 and %d\n", buf->size);
  }
  float value = luaL_checknumber(L, 3);
  buf->values[index - 1] = value;
  return 0;
}

static int buffer_float_len(lua_State *L) {
  Buffer_float* buf = luaL_checkudata(L, 1, "Sunvox.buffer_float");
  lua_pushinteger(L, buf->size);
  return 1;
}

static const struct luaL_Reg SunvoxBuffer_float [] = {
  { "__index",  buffer_float_index },
  { "__newindex",  buffer_float_newindex },
  { "__len",  buffer_float_len },
  { NULL, NULL }
};

static int new_buffer_float(lua_State *L) {
  int n = luaL_checkinteger(L, 1);
  if (n < 0) {
    return luaL_error(L, "Bad buffer size: %d\n", n);
  }
  size_t nbytes = sizeof(Buffer_float) + (n - 1) * sizeof(float);
  Buffer_float *buf = (Buffer_float *)lua_newuserdata(L, nbytes);
  buf->size = n;
  memset(buf->values, 0, sizeof(buf->values));

  luaL_getmetatable(L, "Sunvox.buffer_float");
  lua_setmetatable(L, -2);
  return 1;
}


typedef struct Buffer_int16_t {
  size_t size;
  int16_t values[1];
} Buffer_int16_t;

// metatable method for handling "buffer[index]"
static int buffer_int16_t_index(lua_State* L) {
  Buffer_int16_t* buf = luaL_checkudata(L, 1, "Sunvox.buffer_int16_t");
  int index = luaL_checkinteger(L, 2);
  if (index < 1 || index > buf->size) {
    return luaL_error(L, "Sunvox buffer access error: index should be between 1 and %d\n", buf->size);
  }
  lua_pushnumber(L, buf->values[index - 1]);
  return 1;
}

// metatable method for handle "buffer[index] = value"
static int buffer_int16_t_newindex(lua_State *L) {
  Buffer_int16_t* buf = luaL_checkudata(L, 1, "Sunvox.buffer_int16_t");
  int index = luaL_checkinteger(L, 2);
  if (index < 1 || index > buf->size) {
    return luaL_error(L, "Sunvox buffer access error: index should be between 1 and %d\n", buf->size);
  }
  int16_t value = luaL_checknumber(L, 3);
  buf->values[index - 1] = value;
  return 0;
}

static int buffer_int16_t_len(lua_State *L) {
  Buffer_int16_t* buf = luaL_checkudata(L, 1, "Sunvox.buffer_int16_t");
  lua_pushinteger(L, buf->size);
  return 1;
}

static const struct luaL_Reg SunvoxBuffer_int16_t [] = {
  { "__index",  buffer_int16_t_index },
  { "__newindex",  buffer_int16_t_newindex },
  { "__len",  buffer_int16_t_len },
  { NULL, NULL }
};

static int new_buffer_int16_t(lua_State *L) {
  int n = luaL_checkinteger(L, 1);
  if (n < 0) {
    return luaL_error(L, "Bad buffer size: %d\n", n);
  }
  size_t nbytes = sizeof(Buffer_int16_t) + (n - 1) * sizeof(int16_t);
  Buffer_int16_t *buf = (Buffer_int16_t *)lua_newuserdata(L, nbytes);
  buf->size = n;
  memset(buf->values, 0, sizeof(buf->values));

  luaL_getmetatable(L, "Sunvox.buffer_int16_t");
  lua_setmetatable(L, -2);
  return 1;
}


static int new_buffer(lua_State *L) {
  if (use_int16_t) {
    return new_buffer_int16_t(L);
  } else {
    return new_buffer_float(L);
  }
}
static int lua_sv_audio_callback(lua_State* L) {
  int lua_frames = luaL_checkinteger(L, 2);
  int lua_latency = luaL_checkinteger(L, 3);
  uint32_t lua_out_time = check_unsigned_int(L, 4);

  void *cbuf;

  if (use_int16_t) {
    Buffer_int16_t *buf = luaL_checkudata(L, 1, "Sunvox.buffer_int16_t");
    cbuf = &(buf->values);
  } else {
    Buffer_float *buf = luaL_checkudata(L, 1, "Sunvox.buffer_float");
    cbuf = &(buf->values);
  }

  int lua_ret = sv_audio_callback(
    cbuf,
    lua_frames,
    lua_latency,
    lua_out_time
  );

  lua_pushinteger(L, lua_ret);
  return 1;
}

static int lua_sv_audio_callback2(lua_State* L) {
    int lua_frames = luaL_checkinteger(L, 2);
    int lua_latency = luaL_checkinteger(L, 3);
    uint32_t lua_out_time = check_unsigned_int(L, 4);
    int lua_in_type = luaL_checkinteger(L, 5);
    int lua_in_channels = luaL_checkinteger(L, 6);

    void *c_buf, *c_in_buf;

    if (use_int16_t) {
      Buffer_int16_t *buf = luaL_checkudata(L, 1, "Sunvox.buffer_int16_t");
      Buffer_int16_t *in_buf = luaL_checkudata(L, 1, "Sunvox.buffer_int16_t");
      c_buf = &(buf->values);
      c_in_buf = &(buf->values);
    } else {
      Buffer_float *buf = luaL_checkudata(L, 1, "Sunvox.buffer_float");
      Buffer_float *in_buf = luaL_checkudata(L, 1, "Sunvox.buffer_float");
      c_buf = &(buf->values);
      c_in_buf = &(buf->values);
    }

    int lua_ret = sv_audio_callback2(
      c_buf,
      lua_frames,
      lua_latency,
      lua_out_time,
      lua_in_type,
      lua_in_channels,
      c_in_buf
    );

    lua_pushinteger(L, lua_ret);
    return 1;
  }

static int lua_sv_open_slot(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int ret = sv_open_slot(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_close_slot(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int ret = sv_close_slot(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_lock_slot(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int ret = sv_lock_slot(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_unlock_slot(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int ret = sv_unlock_slot(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int is_flag_set_true(lua_State *L, int nparam, const char *key) {
  lua_pushstring(L, key);
  lua_gettable(L, nparam);
  int ltype = lua_type(L, -1);
  if (ltype == LUA_TNIL) {
    return 0;
  } else if (ltype == LUA_TBOOLEAN) {
    return lua_toboolean(L, -1);
  } else {
    return 1;
  }
}

static int get_int_param(lua_State *L, int nparam, const char *key) {
  lua_pushstring(L, key);
  lua_gettable(L, nparam);
  int ltype = lua_type(L, -1);
  if (ltype == LUA_TNIL) {
    return 0;
  } else if (ltype == LUA_TNUMBER) {
    return luaL_checkinteger(L, -1);
  } else {
    luaL_error(L, "Bad parameter type for '%s': expected number, got %s\n", key, lua_typename(L, ltype));
    return 0; // won't get there tbh
  }
}

/*
From SunVox docs:

SV_INIT_FLAG_NO_DEBUG_OUTPUT;
SV_INIT_FLAG_USER_AUDIO_CALLBACK - offline mode: system-dependent audio stream will not be created; user must call sv_audio_callback() to get the next piece of sound stream;
SV_INIT_FLAG_OFFLINE - same as SV_INIT_FLAG_USER_AUDIO_CALLBACK;
SV_INIT_FLAG_AUDIO_INT16 - desired sample type of the output sound stream : int16_t;
SV_INIT_FLAG_AUDIO_FLOAT32 - desired sample type of the output sound stream : float; the actual sample type may be different, if SV_INIT_FLAG_USER_AUDIO_CALLBACK is not set;
SV_INIT_FLAG_ONE_THREAD - audio callback and song modification are in single thread; use it with SV_INIT_FLAG_USER_AUDIO_CALLBACK only.
*/
static int get_flags(lua_State *L, int nparam) {
  int flags = 0;
  if (is_flag_set_true(L, nparam, "no_debug_output")) {
    flags |= SV_INIT_FLAG_NO_DEBUG_OUTPUT;
  }
  if (is_flag_set_true(L, nparam, "offline")) {
    flags |= SV_INIT_FLAG_OFFLINE;
  }
  // We don't cover "user_audio_callback" as it's the same as "offline"
  if (is_flag_set_true(L, nparam, "one_thread")) {
    flags |= SV_INIT_FLAG_ONE_THREAD;
  }

  int sample_type = get_int_param(L, nparam, "sample_type");
  if (sample_type == 16) {
    use_int16_t = 1;
    flags |= SV_INIT_FLAG_AUDIO_INT16;
  } else if (sample_type == 32) {
    use_int16_t = 0;
    flags |= SV_INIT_FLAG_AUDIO_FLOAT32;
  } else if (sample_type != 0) {
    luaL_error(L, "Parameter \"sample_type\" can be either 16 or 32, not %d\n", sample_type);
  }
  return flags;
}

static int lua_sv_init(lua_State *L) {
  const char* config = luaL_checkstring(L, 1);
  int freq = luaL_checkinteger(L, 2);
  int channels = luaL_checkinteger(L, 3);
  // Let's suppose we have params table instead of flags
  luaL_checktype(L, 4, LUA_TTABLE);

  uint32_t flags = get_flags(L, 4);

  // use_int16_t is defined on top of compiled file
  // see luavox.c when it's built
  if (flags & SV_INIT_FLAG_AUDIO_INT16) {
    use_int16_t = 1;
  } else if (flags & SV_INIT_FLAG_AUDIO_FLOAT32) {
    use_int16_t = 0;
  }

  int ret = sv_init(config, freq, channels, flags);
  lua_pushinteger(L, ret);
  return 1;
}

static int lua_sv_deinit(lua_State *L) {
  int ret = sv_deinit();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_sample_rate(lua_State *L) {
  int ret = sv_get_sample_rate();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_update_input(lua_State *L) {
  int ret = sv_update_input();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_load(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  const char* lua_name = luaL_checkstring(L, 2);
  int ret = sv_load(lua_slot, lua_name);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_play(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int ret = sv_play(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_play_from_beginning(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int ret = sv_play_from_beginning(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_stop(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int ret = sv_stop(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_pause(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int ret = sv_pause(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_resume(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int ret = sv_resume(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_set_autostop(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_autostop = luaL_checkinteger(L, 2);
  int ret = sv_set_autostop(lua_slot, lua_autostop);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_autostop(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int ret = sv_get_autostop(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_end_of_song(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int ret = sv_end_of_song(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_rewind(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_t = luaL_checkinteger(L, 2);
  int ret = sv_rewind(lua_slot, lua_t);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_volume(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_vol = luaL_checkinteger(L, 2);
  int ret = sv_volume(lua_slot, lua_vol);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_set_event_t(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_set = luaL_checkinteger(L, 2);
  int lua_t = luaL_checkinteger(L, 3);
  int ret = sv_set_event_t(lua_slot, lua_set, lua_t);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_send_event(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_track_num = luaL_checkinteger(L, 2);
  int lua_note = luaL_checkinteger(L, 3);
  int lua_vel = luaL_checkinteger(L, 4);
  int lua_module = luaL_checkinteger(L, 5);
  int lua_ctl = luaL_checkinteger(L, 6);
  int lua_ctl_val = luaL_checkinteger(L, 7);
  int ret = sv_send_event(lua_slot, lua_track_num, lua_note, lua_vel, lua_module, lua_ctl, lua_ctl_val);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_current_line(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int ret = sv_get_current_line(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_current_line2(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int ret = sv_get_current_line2(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_current_signal_level(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_channel = luaL_checkinteger(L, 2);
  int ret = sv_get_current_signal_level(lua_slot, lua_channel);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_song_name(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  const char* ret = sv_get_song_name(lua_slot);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_sv_get_song_bpm(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int ret = sv_get_song_bpm(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_song_tpl(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int ret = sv_get_song_tpl(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_song_length_frames(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  uint32_t ret = sv_get_song_length_frames(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_song_length_lines(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  uint32_t ret = sv_get_song_length_lines(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_time_map(lua_State* L) {
    int lua_slot = luaL_checkinteger(L, 1);
    int lua_start_line = luaL_checkinteger(L, 2);
    int lua_len = luaL_checkinteger(L, 3);

    uint32_t* lua_dest = (uint32_t*)calloc(lua_len, sizeof(uint32_t));

    int lua_flags = luaL_checkinteger(L, 5);

    int lua_ret = sv_get_time_map(
      lua_slot,
      lua_start_line,
      lua_len,
      lua_dest,
      lua_flags
    );
    // lua stack: slot, line, len, {}, flags
    for (int i = 0; i < lua_len; i++) {
      lua_pushinteger(L, i + 1);
      lua_pushinteger(L, lua_dest[i]);
      // lua stack: slot, line, len, {}, flags, i, value
      lua_settable(L, -4);
    }
    lua_pushinteger(L, lua_ret);
    free(lua_dest);

    return 1;
  }

static int lua_sv_new_module(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  const char* lua_type = luaL_checkstring(L, 2);
  const char* lua_name = luaL_checkstring(L, 3);
  int lua_x = luaL_checkinteger(L, 4);
  int lua_y = luaL_checkinteger(L, 5);
  int lua_z = luaL_checkinteger(L, 6);
  int ret = sv_new_module(lua_slot, lua_type, lua_name, lua_x, lua_y, lua_z);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_remove_module(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_mod_num = luaL_checkinteger(L, 2);
  int ret = sv_remove_module(lua_slot, lua_mod_num);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_connect_module(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_source = luaL_checkinteger(L, 2);
  int lua_destination = luaL_checkinteger(L, 3);
  int ret = sv_connect_module(lua_slot, lua_source, lua_destination);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_disconnect_module(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_source = luaL_checkinteger(L, 2);
  int lua_destination = luaL_checkinteger(L, 3);
  int ret = sv_disconnect_module(lua_slot, lua_source, lua_destination);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_load_module(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  const char* lua_file_name = luaL_checkstring(L, 2);
  int lua_x = luaL_checkinteger(L, 3);
  int lua_y = luaL_checkinteger(L, 4);
  int lua_z = luaL_checkinteger(L, 5);
  int ret = sv_load_module(lua_slot, lua_file_name, lua_x, lua_y, lua_z);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_sampler_load(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_sampler_module = luaL_checkinteger(L, 2);
  const char* lua_file_name = luaL_checkstring(L, 3);
  int lua_sample_slot = luaL_checkinteger(L, 4);
  int ret = sv_sampler_load(lua_slot, lua_sampler_module, lua_file_name, lua_sample_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_number_of_modules(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int ret = sv_get_number_of_modules(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_find_module(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  const char* lua_name = luaL_checkstring(L, 2);
  int ret = sv_find_module(lua_slot, lua_name);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_module_flags(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_mod_num = luaL_checkinteger(L, 2);
  uint32_t ret = sv_get_module_flags(lua_slot, lua_mod_num);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_module_inputs(lua_State* L) {
    int slot = luaL_checkinteger(L, 1);
    int mod_num = luaL_checkinteger(L, 2);

    uint32_t module_flags = sv_get_module_flags(slot, mod_num);

    int* ret = sv_get_module_inputs(slot, mod_num);

    size_t number_of_inputs = (module_flags & SV_MODULE_INPUTS_MASK) >> SV_MODULE_INPUTS_OFF;

    lua_newtable(L);
    for (int i = 0; i < number_of_inputs; i++) {
      lua_pushinteger(L, i + 1);
      lua_pushinteger(L, ret[i]);
      lua_settable(L, -3);
    }

    return 1;
  }

static int lua_sv_get_module_outputs(lua_State* L) {
  int slot = luaL_checkinteger(L, 1);
  int mod_num = luaL_checkinteger(L, 2);

  uint32_t module_flags = sv_get_module_flags(slot, mod_num);

  int* ret = sv_get_module_outputs(slot, mod_num);

  size_t number_of_outputs = (module_flags & SV_MODULE_OUTPUTS_MASK) >> SV_MODULE_OUTPUTS_OFF;

  lua_newtable(L);
  for (int i = 0; i < number_of_outputs; i++) {
    lua_pushinteger(L, i + 1);
    lua_pushinteger(L, ret[i]);
    lua_settable(L, -3);
  }

  return 1;
}

static int lua_sv_get_module_name(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_mod_num = luaL_checkinteger(L, 2);
  const char* ret = sv_get_module_name(lua_slot, lua_mod_num);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_sv_get_module_xy(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_mod_num = luaL_checkinteger(L, 2);
  uint32_t ret = sv_get_module_xy(lua_slot, lua_mod_num);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_module_color(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_mod_num = luaL_checkinteger(L, 2);
  int ret = sv_get_module_color(lua_slot, lua_mod_num);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_module_finetune(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_mod_num = luaL_checkinteger(L, 2);
  uint32_t ret = sv_get_module_finetune(lua_slot, lua_mod_num);
  lua_pushinteger(L, ret);
  return 1;
}
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

static int lua_sv_get_number_of_module_ctls(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_mod_num = luaL_checkinteger(L, 2);
  int ret = sv_get_number_of_module_ctls(lua_slot, lua_mod_num);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_module_ctl_name(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_mod_num = luaL_checkinteger(L, 2);
  int lua_ctl_num = luaL_checkinteger(L, 3);
  const char* ret = sv_get_module_ctl_name(lua_slot, lua_mod_num, lua_ctl_num);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_sv_get_module_ctl_value(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_mod_num = luaL_checkinteger(L, 2);
  int lua_ctl_num = luaL_checkinteger(L, 3);
  int lua_scaled = luaL_checkinteger(L, 4);
  int ret = sv_get_module_ctl_value(lua_slot, lua_mod_num, lua_ctl_num, lua_scaled);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_number_of_patterns(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int ret = sv_get_number_of_patterns(lua_slot);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_find_pattern(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  const char* lua_name = luaL_checkstring(L, 2);
  int ret = sv_find_pattern(lua_slot, lua_name);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_pattern_x(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_pat_num = luaL_checkinteger(L, 2);
  int ret = sv_get_pattern_x(lua_slot, lua_pat_num);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_pattern_y(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_pat_num = luaL_checkinteger(L, 2);
  int ret = sv_get_pattern_y(lua_slot, lua_pat_num);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_pattern_tracks(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_pat_num = luaL_checkinteger(L, 2);
  int ret = sv_get_pattern_tracks(lua_slot, lua_pat_num);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_pattern_lines(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_pat_num = luaL_checkinteger(L, 2);
  int ret = sv_get_pattern_lines(lua_slot, lua_pat_num);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_pattern_name(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_pat_num = luaL_checkinteger(L, 2);
  const char* ret = sv_get_pattern_name(lua_slot, lua_pat_num);
  lua_pushstring(L, ret);
  return 1;
}
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

static int lua_sv_pattern_mute(lua_State *L) {
  int lua_slot = luaL_checkinteger(L, 1);
  int lua_pat_num = luaL_checkinteger(L, 2);
  int lua_mute = luaL_checkinteger(L, 3);
  int ret = sv_pattern_mute(lua_slot, lua_pat_num, lua_mute);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_ticks(lua_State *L) {
  uint32_t ret = sv_get_ticks();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_ticks_per_second(lua_State *L) {
  uint32_t ret = sv_get_ticks_per_second();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_sv_get_log(lua_State *L) {
  int lua_size = luaL_checkinteger(L, 1);
  const char* ret = sv_get_log(lua_size);
  lua_pushstring(L, ret);
  return 1;
}
static const struct luaL_Reg luavox [] = {
  { "audio_callback", lua_sv_audio_callback},
  { "audio_callback2", lua_sv_audio_callback2},
  { "open_slot", lua_sv_open_slot},
  { "close_slot", lua_sv_close_slot},
  { "lock_slot", lua_sv_lock_slot},
  { "unlock_slot", lua_sv_unlock_slot},
  { "init", lua_sv_init},
  { "deinit", lua_sv_deinit},
  { "get_sample_rate", lua_sv_get_sample_rate},
  { "update_input", lua_sv_update_input},
  { "load", lua_sv_load},
  { "play", lua_sv_play},
  { "play_from_beginning", lua_sv_play_from_beginning},
  { "stop", lua_sv_stop},
  { "pause", lua_sv_pause},
  { "resume", lua_sv_resume},
  { "set_autostop", lua_sv_set_autostop},
  { "get_autostop", lua_sv_get_autostop},
  { "end_of_song", lua_sv_end_of_song},
  { "rewind", lua_sv_rewind},
  { "volume", lua_sv_volume},
  { "set_event_t", lua_sv_set_event_t},
  { "send_event", lua_sv_send_event},
  { "get_current_line", lua_sv_get_current_line},
  { "get_current_line2", lua_sv_get_current_line2},
  { "get_current_signal_level", lua_sv_get_current_signal_level},
  { "get_song_name", lua_sv_get_song_name},
  { "get_song_bpm", lua_sv_get_song_bpm},
  { "get_song_tpl", lua_sv_get_song_tpl},
  { "get_song_length_frames", lua_sv_get_song_length_frames},
  { "get_song_length_lines", lua_sv_get_song_length_lines},
  { "get_time_map", lua_sv_get_time_map},
  { "new_module", lua_sv_new_module},
  { "remove_module", lua_sv_remove_module},
  { "connect_module", lua_sv_connect_module},
  { "disconnect_module", lua_sv_disconnect_module},
  { "load_module", lua_sv_load_module},
  { "sampler_load", lua_sv_sampler_load},
  { "get_number_of_modules", lua_sv_get_number_of_modules},
  { "find_module", lua_sv_find_module},
  { "get_module_flags", lua_sv_get_module_flags},
  { "get_module_inputs", lua_sv_get_module_inputs},
  { "get_module_outputs", lua_sv_get_module_outputs},
  { "get_module_name", lua_sv_get_module_name},
  { "get_module_xy", lua_sv_get_module_xy},
  { "get_module_color", lua_sv_get_module_color},
  { "get_module_finetune", lua_sv_get_module_finetune},
  { "get_module_scope2", lua_sv_get_module_scope2},
  { "module_curve", lua_sv_module_curve},
  { "get_number_of_module_ctls", lua_sv_get_number_of_module_ctls},
  { "get_module_ctl_name", lua_sv_get_module_ctl_name},
  { "get_module_ctl_value", lua_sv_get_module_ctl_value},
  { "get_number_of_patterns", lua_sv_get_number_of_patterns},
  { "find_pattern", lua_sv_find_pattern},
  { "get_pattern_x", lua_sv_get_pattern_x},
  { "get_pattern_y", lua_sv_get_pattern_y},
  { "get_pattern_tracks", lua_sv_get_pattern_tracks},
  { "get_pattern_lines", lua_sv_get_pattern_lines},
  { "get_pattern_name", lua_sv_get_pattern_name},
  { "get_pattern_data", lua_sv_get_pattern_data},
  { "pattern_mute", lua_sv_pattern_mute},
  { "get_ticks", lua_sv_get_ticks},
  { "get_ticks_per_second", lua_sv_get_ticks_per_second},
  { "get_log", lua_sv_get_log},
  { "sunvox_buffer", new_buffer },
  { NULL, NULL }

};


int luaopen_luavox(lua_State *L) {
  luaL_newmetatable(L, "Sunvox.note");
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, SunvoxNote, 0);

  luaL_newmetatable(L, "Sunvox.buffer_int16_t");
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, SunvoxBuffer_int16_t, 0);

  luaL_newmetatable(L, "Sunvox.buffer_float");
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, SunvoxBuffer_float, 0);

  luaL_newlib(L, luavox);
  sv_load_dll();
  return 1;
}


#ifdef __cplusplus
}
#endif
