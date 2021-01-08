all: build

build: c_source shared_lib

c_source:
	lua5.3 main.lua sunvox.h build/luavox.c

shared_lib:
	gcc -I/usr/include/lua5.3 -I./ -llua5.3 -fPIC build/luavox.c -shared -o build/luavox.so
# gcc -I/usr/include/luajit-2.1 -I./ -lluajit-5.1 -fPIC build/luavox.c -shared -o build/luavox.so

test: build
	cp -n build/sunvox.so spec/
	cp build/luavox.so spec/
	busted -C spec/ .
