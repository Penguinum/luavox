all: build

build: build_c_source build_lib

build_c_source:
	lua5.3 main.lua sunvox.h luavox.c

build_lib:
	gcc -D_GNU_SOURCE -I/usr/include/lua5.3 -I./ -llua5.3 -fPIC luavox.c -shared -o luavox.so

