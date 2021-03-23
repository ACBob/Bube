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
	int result;
	result = luaL_loadfile(l, fp);
	if (result) // 0 -> OK
	{
		if (msg) conoutf(CON_ERROR, "LUA ERROR:\n%s",luaapi_geterror());
		return false;
	}

	lua_call(l, 0, 0);

	return true;
}

// The command to run Lua files.
void luadofile(const char *fp) {
	luaapi_dofile(fp, true);
}
COMMAND(luadofile, "s");

// Some lua functions.
// Intended to only be called from lua!
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

	// Expose the lua api functions
	lua_register(l, "log", lualog);

	return true;
}