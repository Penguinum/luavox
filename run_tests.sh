#!/bin/sh

cp -n sunvox_lib/headers/sunvox.h spec/
cp -n sunvox_lib/resources/test.sunvox spec/
cp build/libluavox.so spec/luavox.so
busted -C spec/ .
