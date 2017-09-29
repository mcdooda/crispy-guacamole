#include "game.h"
#include "../gamestate.h"
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

		{"openMap", l_Game_openMap},

		{nullptr, nullptr}
	};
	luaL_setfuncs(L, Game_lib_f, 0);
	lua_setglobal(L, "Game");

	return 0;
}

int l_Game_getTime(lua_State* L)
{
	BaseState& baseState = getBaseState(L);
	flat::time::Clock& clock = baseState.getClock();
	lua_pushnumber(L, clock.getTime());
	return 1;
}

int l_Game_openMap(lua_State* L)
{
	const char* modPath = luaL_checkstring(L, 1);
	const char* mapName = luaL_checkstring(L, 2);
	Game& game = flat::lua::getFlatAs<Game>(L);
	game.modPath = modPath;
	game.mapName = mapName;
	std::unique_ptr<GameState> gameState = std::make_unique<GameState>();
	gameState->setModPath(modPath);
	game.getStateMachine().setNextState(std::move(gameState));
	return 1;
}

BaseState& getBaseState(lua_State* L)
{
	Game& game = flat::lua::getFlatAs<Game>(L);
	return game.getStateMachine().getState()->to<BaseState>();
}

// private


} // game
} // lua
} // states
} // game