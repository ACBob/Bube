#ifndef LUAAPI_H
#define LUAAPI_H

#include <lua5.1/lua.hpp>

extern lua_State *l;

bool init_luaapi();
const char *luaapi_geterror();
bool luaapi_dofile(const char *fp);

#endif // LUAAPI_H