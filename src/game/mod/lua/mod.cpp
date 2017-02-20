#include "mod.h"
#include "../../game.h"

namespace game
{
namespace mod
{
namespace lua
{

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_createtable(L, 0, 1);
	static const luaL_Reg Mod_lib_m[] = {
		{"getPath",           l_Mod_getPath},
		{"getCurrentMapName", l_Mod_getCurrentMapName},
		{nullptr, nullptr}
	};
	luaL_setfuncs(L, Mod_lib_m, 0);
	lua_setglobal(L, "Mod");

	return 0;
}

int l_Mod_getPath(lua_State * L)
{
	Game& game = flat::lua::getGame(L).to<Game>();
	lua_pushstring(L, game.modPath.c_str());
	return 1;
}

int l_Mod_getCurrentMapName(lua_State* L)
{
	Game& game = flat::lua::getGame(L).to<Game>();
	lua_pushstring(L, game.mapName.c_str());
	return 1;
}

} // lua
} // mod
} // game


