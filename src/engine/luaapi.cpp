// Does the Lua!
#include "luaapi.h"

// The global Lua state.
lua_State *l = 0;


bool init_luaapi() {
	// set up a new state
	l = luaL_newstate();

	luaL_openlibs(l); // Open all libraries

	luaL_loadfile(l, "test.lua");
	lua_call(l, 0, 0);

	return true;
}