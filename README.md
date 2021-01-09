# Luavox

## What?
Binding generator for [SunVox library](https://www.warmplace.ru/soft/sunvox/sunvox_lib.php) and the binding itself.

## Why?
Wanna use SunVox library in Lua (probably in some love2d game). It's awesome, allows you doing dynamic/generative music and stuff.

## Okay, how do I use it?
First, build it, okay?
Then put both `luavox.so` and `sunvox.so` in lua libs path (most likely you'll want to put it in project root).
Then:
```lua
local lv = require "luavox"

if lv.init(0, 44100, 2, {}) >= 0 then
    lv.open_slot(0)
    lv.load(0, "Transient - Can It Be Beautiful.sunvox")
    lv.volume(0, 256)
    lv.play_from_beginning(0)
    os.execute("sleep 10") -- sleep 10 seconds
    lv.stop(0)
    lv.close_slot(0)
    lv.deinit()
end
```

## No Windows / Mac yet?
Yep, can't guarantee it'll build on any of them but it should be simple to add support. I managed to cross-compile Windows version with mingw, will add Windows support eventually.

## License?
MIT license for evertything except contents of sunvox_lib. For MIT license see LICENSE.txt, for licenses of sunvox_lib folder content see sunvox_lib/docs.