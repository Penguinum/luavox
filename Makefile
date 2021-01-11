all: build

build: c_source shared_lib

c_source:
	lua5.3 main.lua sunvox_lib/headers/sunvox.h build/luavox.c

shared_lib:
	gcc -I/usr/include/lua5.3 -I./sunvox_lib/headers -llua5.3 -fPIC build/luavox.c -shared -o build/luavox.so

test: build
	cp -n sunvox_lib/headers/sunvox.h spec/
	cp -n sunvox_lib/resources/test.sunvox spec/
	cp build/luavox.so spec/
	busted -C spec/ .
