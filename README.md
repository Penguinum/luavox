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

## Building
```sh
$ sh build_source.sh
$ mkdir build
$ cd build
$ cmake .. -G"Unix Makefiles" # or whatever lower level build system you want to use
$ make
```
In case you're cross-compiling, or CMake can't find Lua,
or you want different version of Lua from what CMake finds for you,
you'll have to provide it some variables.
Example for cross-compiling from Linux to Windows:
```sh
$ cmake .. -G"Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../Win64CrossCompileToolchain\
     -DLUA_LIBRARIES=/path/to/compiled/lua/src \
     -DLUA_INCLUDE_DIR=/path/to/compiled/lua/src \
     -DLUA_LIB=lua # may be something like -DLUA_LIB=luajit-5.1.dll in case of LuaJIT
```
Little bit messy, but it works.
Don't know if it'll work for Mac, most likely will build with no pain.

## License?
MIT license for evertything except contents of sunvox_lib.
For MIT license see LICENSE.txt, for licenses of sunvox_lib folder content see sunvox_lib/docs.