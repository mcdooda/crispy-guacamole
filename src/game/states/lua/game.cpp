#include "game.h"
#include "../basemapstate.h"
#include "../../game.h"

namespace game
{
namespace states
{
namespace lua
{
namespace game
{

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_createtable(L, 0, 1);
	static const luaL_Reg Game_lib_f[] = {
		{"getTime", l_Game_getTime},

		{nullptr, nullptr}
	};
	luaL_setfuncs(L, Game_lib_f, 0);
	lua_setglobal(L, "Game");

	return 0;
}

int l_Game_getTime(lua_State* L)
{
	BaseMapState& baseMapState = getBaseMapState(L);
	flat::time::Clock& clock = baseMapState.getClock();
	lua_pushnumber(L, clock.getTime());
	return 1;
}

BaseMapState& getBaseMapState(lua_State* L)
{
	Game& game = flat::lua::getGame(L).to<Game>();
	return game.getStateMachine().getState()->to<BaseMapState>();
}

// private


} // game
} // lua
} // states
} // game