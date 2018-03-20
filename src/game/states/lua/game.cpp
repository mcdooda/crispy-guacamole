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

		{ "debug_getNumTimers",   l_Game_debug_getNumTimers},

		{"debug_reloadComponent", l_Game_debug_reloadComponent},
		{"debug_removeComponent", l_Game_debug_removeComponent },
#endif

		{"openMap",               l_Game_openMap},

		{"setGhostEntity",        l_Game_setGhostEntity},
		{"clearGhostEntity",      l_Game_clearGhostEntity},

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

int l_Game_debug_getNumTimers(lua_State* L)
{
	BaseState& baseState = base::getBaseState(L);
	flat::time::Clock& clock = baseState.getClock();
	lua_pushinteger(L, baseState.getTimerContainer().size());
	return 1;
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

int l_Game_debug_removeComponent(lua_State* L)
{
	std::string entityTemplateName = luaL_checkstring(L, 1);
	entity::component::ComponentFlags componentFlag = static_cast<entity::component::ComponentFlags>(luaL_checkinteger(L, 2));
	GameState& gameState = base::getBaseState(L).as<GameState>();
	Game& game = flat::lua::getFlatAs<Game>(L);
	std::shared_ptr<const entity::EntityTemplate> entityTemplate = gameState.getEntityTemplate(game, entityTemplateName);
	const_cast<entity::EntityTemplate*>(entityTemplate.get())->removeComponent(game, gameState.getComponentRegistry(), componentFlag);
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

int l_Game_setGhostEntity(lua_State* L)
{
	const char* ghostTemplateName = luaL_checkstring(L, 1);
	flat::lua::UniqueLuaReference<LUA_TFUNCTION> canPlaceGhostEntity;
	canPlaceGhostEntity.setIfNotNil(L, 2);
	flat::lua::UniqueLuaReference<LUA_TFUNCTION> onGhostEntityPlaced;
	onGhostEntityPlaced.setIfNotNil(L, 3);
	Game& game = flat::lua::getFlatAs<Game>(L);
	GameState& gameState = base::getBaseState(L).as<GameState>();
	std::shared_ptr<const entity::EntityTemplate> ghostTemplate = gameState.getEntityTemplate(game, ghostTemplateName);
	gameState.setGhostTemplate(game, ghostTemplate);
	gameState.setCanPlaceGhostEntity(std::move(canPlaceGhostEntity));
	gameState.setOnGhostEntityPlaced(std::move(onGhostEntityPlaced));
	return 0;
}

int l_Game_clearGhostEntity(lua_State* L)
{
	GameState& gameState = base::getBaseState(L).as<GameState>();
	gameState.clearGhostTemplate();
	return 0;
}

} // game
} // lua
} // states
} // game