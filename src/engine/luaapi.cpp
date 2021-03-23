// Does the Lua!
#include "luaapi.h"
#include "engine.h"

// The global Lua state.
lua_State *l = 0;

const char* luaapi_geterror() {
	return lua_tostring(l, -1);
}

bool luaapi_dofile(const char *fp, bool msg)
{
	if (luaL_dofile(l, fp)) // 0 -> OK, if there's an error the return value != 0
	{
		if (msg) conoutf(CON_ERROR, "LUA ERROR:\n%s",luaapi_geterror());
		return false;
	}

	return true;
}

// The command to run Lua files.
void luadofile(const char *fp) {
	luaapi_dofile(fp, true);
}
COMMAND(luadofile, "s");

// Some lua functions.
// Intended to only be called from lua!
// Most other bube namespace functions are implemented in builtin.lua.
extern "C" int lualog(lua_State *l) {
	const char *msg = lua_tostring(l, 1);
	
	conoutf(CON_INFO, msg);

	return 0;
}



bool init_luaapi() {
	// set up a new state
	l = luaL_newstate();
	if (!l) return false;

	luaL_openlibs(l); // Open all libraries

	// create the bube table/namespace
	lua_newtable(l);
	{
		// Expose the lua api functions
		lua_pushliteral(l, "log");
		lua_pushcfunction(l, lualog);

		lua_settable(l, -3);
	}
	lua_setglobal(l, "bube");

	// now run builtin.lua
	if (!luaapi_dofile("script/builtin.lua", true)) return false;

	return true;
}