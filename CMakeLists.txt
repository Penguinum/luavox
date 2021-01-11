cmake_minimum_required(VERSION 3.16)
project(LuaVox)

find_package(Lua REQUIRED)
include_directories(
    sunvox_lib/headers
    ${LUA_INCLUDE_DIR}
)
link_directories(${LUA_INCLUDE_DIR})
set(SOURCES luavox.c)
add_library(luavox SHARED ${SOURCES} ${HEADERS})
target_link_libraries(luavox ${LUA_LIB})
