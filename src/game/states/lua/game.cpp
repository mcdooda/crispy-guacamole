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
	static const luaL_Reg game_lib_f[] = {
		{"getTime",                    l_game_getTime},
		{"getDT",                      l_game_getDT},
		{"Timer",                      l_game_Timer},

#ifdef FLAT_DEBUG
		{"debug_setTimeSpeed",         l_game_debug_setTimeSpeed},
		{"debug_pause",                l_game_debug_pause},
		{"debug_resume",               l_game_debug_resume},
		{"debug_pauseNextFrame",       l_game_debug_pauseNextFrame},

		{"debug_getNumTimers",         l_game_debug_getNumTimers},
		{"debug_getNumFrameTimers",    l_game_debug_getNumFrameTimers},

		{"debug_reloadComponent",      l_game_debug_reloadComponent},
		{"debug_removeComponent",      l_game_debug_removeComponent},
#endif

		{"setCameraCenter",            l_game_setCameraCenter},
		{"getCameraCenter",            l_game_getCameraCenter},
		{"setCameraZoom",              l_game_setCameraZoom},
		{"lockCamera",                 l_game_lockCamera},
		{"unlockCamera",               l_game_unlockCamera},
		{"convertToCameraPosition",    l_game_convertToCameraPosition},

		{"openMap",                    l_game_openMap},

		{"setGhostEntity",             l_game_setGhostEntity},
		{"clearGhostEntity",           l_game_clearGhostEntity},

		{nullptr, nullptr}
	};
	luaL_setfuncs(L, game_lib_f, 0);
	lua_setglobal(L, "game");

	return 0;
}

int l_game_getTime(lua_State* L)
{
	BaseMapState& baseMapState = base::getBaseState(L).to<BaseMapState>();
	flat::time::Clock& clock = baseMapState.getGameClock();
	lua_pushnumber(L, clock.getTime());
	return 1;
}

int l_game_getDT(lua_State* L)
{
	BaseMapState& baseMapState = base::getBaseState(L).to<BaseMapState>();
	flat::time::Clock& clock = baseMapState.getGameClock();
	lua_pushnumber(L, clock.getDT());
	return 1;
}

int l_game_Timer(lua_State* L)
{
	BaseMapState& baseMapState = base::getBaseState(L).to<BaseMapState>();
	flat::lua::timer::TimerContainer& timerContainer = baseMapState.getGameTimerContainer();
	flat::lua::timer::lua::pushTimer(L, timerContainer.add());
	return 1;
}

#ifdef FLAT_DEBUG
int l_game_debug_setTimeSpeed(lua_State* L)
{
	float timeSpeed = static_cast<float>(luaL_checknumber(L, 1));
	BaseMapState& baseMapState = base::getBaseState(L).to<BaseMapState>();
	flat::time::Clock& clock = baseMapState.getGameClock();
	clock.setDTModifier(timeSpeed);
	return 0;
}

int l_game_debug_pause(lua_State* L)
{
	Game& game = flat::lua::getFlatAs<Game>(L);
	BaseMapState& baseMapState = game.getStateMachine().getState()->to<BaseMapState>();
	baseMapState.setGamePause(game, true, false);
	return 0;
}

int l_game_debug_resume(lua_State* L)
{
	Game& game = flat::lua::getFlatAs<Game>(L);
	BaseMapState& baseMapState = game.getStateMachine().getState()->to<BaseMapState>();
	baseMapState.setGamePause(game, false, false);
	return 0;
}

int l_game_debug_pauseNextFrame(lua_State* L)
{
	Game& game = flat::lua::getFlatAs<Game>(L);
	BaseMapState& baseMapState = game.getStateMachine().getState()->to<BaseMapState>();
	baseMapState.setGamePause(game, false, true);
	return 0;
}

int l_game_debug_getNumTimers(lua_State* L)
{
	BaseMapState& baseMapState = base::getBaseState(L).to<BaseMapState>();
	lua_pushinteger(L, baseMapState.getGameTimerContainer().getNumTimers());
	return 1;
}

int l_game_debug_getNumFrameTimers(lua_State* L)
{
	BaseMapState& baseMapState = base::getBaseState(L).to<BaseMapState>();
	lua_pushinteger(L, baseMapState.getGameTimerContainer().getNumFrameTimers());
	return 1;
}

int l_game_debug_reloadComponent(lua_State* L)
{
	std::string entityTemplateName = luaL_checkstring(L, 1);
	entity::component::ComponentFlags componentFlag = static_cast<entity::component::ComponentFlags>(luaL_checkinteger(L, 2));
	bool isNew = lua_toboolean(L, 3) == 1;
	Game& game = flat::lua::getFlatAs<Game>(L);
	BaseMapState& baseMapState = game.getStateMachine().getState()->to<BaseMapState>();
	std::shared_ptr<const entity::EntityTemplate> entityTemplate = baseMapState.getEntityTemplate(game, entityTemplateName);
	const_cast<entity::EntityTemplate*>(entityTemplate.get())->reloadComponent(game, baseMapState.getComponentRegistry(), componentFlag, isNew);
	return 0;
}

int l_game_debug_removeComponent(lua_State* L)
{
	std::string entityTemplateName = luaL_checkstring(L, 1);
	entity::component::ComponentFlags componentFlag = static_cast<entity::component::ComponentFlags>(luaL_checkinteger(L, 2));
	Game& game = flat::lua::getFlatAs<Game>(L);
	BaseMapState& baseMapState = game.getStateMachine().getState()->to<BaseMapState>();
	std::shared_ptr<const entity::EntityTemplate> entityTemplate = baseMapState.getEntityTemplate(game, entityTemplateName);
	const_cast<entity::EntityTemplate*>(entityTemplate.get())->removeComponent(game, baseMapState.getComponentRegistry(), componentFlag);
	return 0;
}
#endif // FLAT_DEBUG

int l_game_openMap(lua_State* L)
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

int l_game_setGhostEntity(lua_State* L)
{
	const char* ghostTemplateName = luaL_checkstring(L, 1);
	flat::lua::UniqueLuaReference<LUA_TFUNCTION> ghostEntitiesPositions;
	ghostEntitiesPositions.setIfNotNil(L, 2);
	flat::lua::UniqueLuaReference<LUA_TFUNCTION> onGhostEntityPlaced;
	onGhostEntityPlaced.setIfNotNil(L, 3);
	Game& game = flat::lua::getFlatAs<Game>(L);
	GameState& gameState = game.getStateMachine().getState()->to<GameState>();
	std::shared_ptr<const entity::EntityTemplate> ghostTemplate = gameState.getEntityTemplate(game, ghostTemplateName);
	gameState.setGhostTemplate(game, ghostTemplate);
	gameState.setGhostEntitiesPositions(std::move(ghostEntitiesPositions));
	gameState.setOnGhostEntityPlaced(std::move(onGhostEntityPlaced));
	return 0;
}

int l_game_clearGhostEntity(lua_State* L)
{
	BaseMapState& baseMapState = base::getBaseState(L).to<BaseMapState>();
	baseMapState.clearGhostTemplate();
	return 0;
}

int l_game_setCameraCenter(lua_State* L)
{
	const flat::Vector2& position = flat::lua::getVector2(L, 1);
	BaseMapState& baseMapState = base::getBaseState(L).to<BaseMapState>();
	baseMapState.setCameraCenter(position);
	return 0;
}

int l_game_getCameraCenter(lua_State* L)
{
	BaseMapState& baseMapState = base::getBaseState(L).to<BaseMapState>();
	const flat::Vector2& position = baseMapState.getCameraCenter();
	flat::lua::pushVector2(L, position);
	return 1;
}

int l_game_setCameraZoom(lua_State* L)
{
	const float zoom = static_cast<float>(luaL_checknumber(L, 1));
	BaseMapState& baseMapState = base::getBaseState(L).to<BaseMapState>();
	baseMapState.setCameraZoom(zoom);
	return 0;
}

int l_game_lockCamera(lua_State* L)
{
	BaseMapState& baseMapState = base::getBaseState(L).to<BaseMapState>();
	baseMapState.lockCamera();
	return 0;
}

int l_game_unlockCamera(lua_State* L)
{
	BaseMapState& baseMapState = base::getBaseState(L).to<BaseMapState>();
	baseMapState.unlockCamera();
	return 0;
}

int l_game_convertToCameraPosition(lua_State* L)
{
	const flat::Vector3& position = flat::lua::getVector3(L, 1);
	BaseMapState& baseMapState = base::getBaseState(L).to<BaseMapState>();
	flat::lua::pushVector2(L, baseMapState.convertToCameraPosition(position));
	return 1;
}

} // game
} // lua
} // states
} // game