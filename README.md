# Luavox

## What?
Binding generator for [SunVox library](https://www.warmplace.ru/soft/sunvox/sunvox_lib.php) and the binding itself.

## Why?
Wanna use SunVox library in Lua (probably in some love2d game). It's awesome, allows you doing dynamic/generative music and cool stuff like that.

## Okay, how do I use it?
*first build*
```lua
local luavox = require "luavox"

sunvox.sv_open_slot(0)
sunvox.sv_load(0, "Transient - Can It Be Beautiful.sunvox")
sunvox.sv_volume(0, 256)
sunvox.sv_play_from_beginning(0)
os.execute("sleep 10") -- sleep 10 seconds
sunvox.sv_stop(0)
sunvox.sv_close_slot(0)
sunvox.sv_deinit()
```

## License?
The SunVox library itself is proprietary, but it seems MIT will work for the binding. TODO yet though, still unsure.