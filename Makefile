all: build

build:
	lua5.3 main.lua sunvox.h luavox.c
# gcc -D_GNU_SOURCE -I/usr/include/lua5.3 -I./sunvox_lib/headers -llua5.3 -fPIC luavox.c -shared -o luavox.so

