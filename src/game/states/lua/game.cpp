#include "game.h"
#include "base.h"
#include "../gamestate.h"
#include "../../game.h"
#include "../../entity/component/componenttype.h"
#include "../../entity/entitytemplate.h"

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
		{"getTime",      l_Game_getTime},

#ifdef FLAT_DEBUG
		{"debug_setTimeSpeed",    l_Game_debug_setTimeSpeed},
		{"debug_pause",           l_Game_debug_pause},
		{"debug_resume",          l_Game_debug_resume},
		{"debug_pauseNextFrame",  l_Game_debug_pauseNextFrame},

		{"debug_reloadComponent", l_Game_debug_reloadComponent},
#endif

		{"openMap",               l_Game_openMap},

		{nullptr, nullptr}
	};
	luaL_setfuncs(L, Game_lib_f, 0);
	lua_setglobal(L, "Game");

	return 0;
}

int l_Game_getTime(lua_State* L)
{
	BaseState& baseState = base::getBaseState(L);
	flat::time::Clock& clock = baseState.getClock();
	lua_pushnumber(L, clock.getTime());
	return 1;
}

#ifdef FLAT_DEBUG
int l_Game_debug_setTimeSpeed(lua_State* L)
{
	float timeSpeed = static_cast<float>(luaL_checknumber(L, 1));
	BaseState& baseState = base::getBaseState(L);
	flat::time::Clock& clock = baseState.getClock();
	clock.setDTModifier(timeSpeed);
	return 0;
}

int l_Game_debug_pause(lua_State* L)
{
	GameState& gameState = base::getBaseState(L).as<GameState>();
	Game& game = flat::lua::getFlatAs<Game>(L);
	gameState.setGamePause(game, true, false);
	return 0;
}

int l_Game_debug_resume(lua_State* L)
{
	GameState& gameState = base::getBaseState(L).as<GameState>();
	Game& game = flat::lua::getFlatAs<Game>(L);
	gameState.setGamePause(game, false, false);
	return 0;
	return 0;
}

int l_Game_debug_pauseNextFrame(lua_State* L)
{
	GameState& gameState = base::getBaseState(L).as<GameState>();
	Game& game = flat::lua::getFlatAs<Game>(L);
	gameState.setGamePause(game, false, true);
	return 0;
}

int l_Game_debug_reloadComponent(lua_State* L)
{
	std::string entityTemplateName = luaL_checkstring(L, 1);
	entity::component::ComponentFlags componentFlag = static_cast<entity::component::ComponentFlags>(luaL_checkinteger(L, 2));
	bool isNew = lua_toboolean(L, 3);
	GameState& gameState = base::getBaseState(L).as<GameState>();
	Game& game = flat::lua::getFlatAs<Game>(L);
	std::shared_ptr<const entity::EntityTemplate> entityTemplate = gameState.getEntityTemplate(game, entityTemplateName);
	const_cast<entity::EntityTemplate*>(entityTemplate.get())->reloadComponent(game, gameState.getComponentRegistry(), componentFlag, isNew);
	return 0;
}
#endif // FLAT_DEBUG

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

} // game
} // lua
} // states
} // game