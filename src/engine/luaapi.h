#ifndef LUAAPI_H
#define LUAAPI_H

#include <lua5.1/lua.hpp>

extern lua_State *l;

bool init_luaapi();
const char* luaapi_geterror();

#endif // LUAAPI_H