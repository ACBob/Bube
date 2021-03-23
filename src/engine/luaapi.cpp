// Does the Lua!
#include "luaapi.h"

// The global Lua state.
lua_State *l = 0;

const char* luaapi_geterror() {
	return lua_tostring(l, -1);
}

bool init_luaapi() {
	// set up a new state
	l = luaL_newstate();
	if (!l) return false;

	luaL_openlibs(l); // Open all libraries

	int r;
	r = luaL_loadfile(l, "test.lua");
	if (r) return false;
	lua_call(l, 0, 0);

	return true;
}