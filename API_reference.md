# API
API is the same as in [original library](https://www.warmplace.ru/soft/sunvox/sunvox_lib.php) with differences listed in this document

## Function names
Every function in SunVox library has prefix "sv_". In this library the prefix was removed as there's no need in it (all functions are namespaced by the library).
So, instead of calling `sv_init` you call `lv.init`.

## Functions
### sv_init
C:
```c
int engine_version = sv_init(const char* config, int sample_rate, int channels, uint32_t flags);
```
Where flags are:
* SV_INIT_FLAG_NO_DEBUG_OUTPUT;
* SV_INIT_FLAG_USER_AUDIO_CALLBACK - offline mode: system-dependent audio stream will not be created; user must call sv_audio_callback() to get the next piece of sound stream;
* SV_INIT_FLAG_OFFLINE - same as SV_INIT_FLAG_USER_AUDIO_CALLBACK;
* SV_INIT_FLAG_AUDIO_INT16 - desired sample type of the output sound stream : int16_t;
* SV_INIT_FLAG_AUDIO_FLOAT32 - desired sample type of the output sound stream : float; the actual sample type may be different, if SV_INIT_FLAG_USER_AUDIO_CALLBACK is not set;
* SV_INIT_FLAG_ONE_THREAD - audio callback and song modification are in single thread; use it with SV_INIT_FLAG_USER_AUDIO_CALLBACK only.

Lua:
```lua
engine_version = lv.init(config, sample_rate, channels, params)
```
Where params are
```lua
{
    no_debug_output = true|false|nil,
    offline = true|false|nil,
    one_thread = true|false|nil
    sample_type = 16|32,
}
```

Not passing `params` works the same way as passing 0 in `sv_init`.

### sv_get_time_map
C:
```c
int return_code = sv_get_time_map(int slot, int start_line, int length, uint32_t* dest, int flags)
```
Where flags are:
* SV_TIME_MAP_SPEED: dest[X] = BPM | ( TPL << 16 ) (speed at the beginning of line X);
* SV_TIME_MAP_FRAMECNT: dest[X] = frame counter at the beginning of line X.

Lua:
```lua
return_code, time_map = lv.get_time_map(slot, start_line, length, params)
```
Where params are
```lua
{
   dest = "speed"|"framecnt" 
}
```
Another difference is dest is not passed into the function but returned from it (as a regular Lua table).

### sv_get_module_flags
C:
```c
uint32_t sv_get_module_flags(int slot, int mod_num);
```
Where flags are:
* SV_MODULE_FLAG_EXISTS;
* SV_MODULE_FLAG_EFFECT;
* SV_MODULE_FLAG_MUTE;
* SV_MODULE_FLAG_SOLO;
* SV_MODULE_FLAG_BYPASS;
* SV_MODULE_INPUTS_OFF;
* SV_MODULE_INPUTS_MASK;
* SV_MODULE_OUTPUTS_OFF;
* SV_MODULE_OUTPUTS_MASK.

Lua:
```lua
flags = lv.get_module_flags(slot, mod_num)
```
Where flags are:
```lua
{
  flag_exists = true|false|nil,
  flag_effect = true|false|nil,
  flag_mute = true|false|nil,
  flag_solo = true|false|nil,
  flag_bypass = true|false|nil,
  inputs_off = true|false|nil,
  inputs_mask = true|false|nil,
  outputs_off = true|false|nil,
  outputs_mask = true|false|nil,
}
```

### sv_audio_callback, sv_audio_callback2
C:
```c
int return_code = sv_audio_callback(void* buf, int frames, int latency, uint32_t out_time);
```

Lua:
```lua
return_code = lv.audio_callback(buf, frames, latency, out_time)
```
Thing to note here is that variable `buf` in Lua code must be userdata created by `lv.sunvox_buffer(len)`.
Same applies to both input and output buffers in sv_audio_callback2.

## Missing functions
These functions miss binding (mainly because I don't see much sense in binding them right now):
* sv_load_from_memory;
* sv_load_module_from_memory;
* sv_sampler_load_from_memory.