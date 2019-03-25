#include "gamestate.h"
#include "../game.h"
#include "../entity/entity.h"
#include "../entity/entitytemplate.h"
#include "../map/brush/lua/tilescontainer.h"

#ifdef FLAT_DEBUG
#include "mapeditorstate.h"
#endif

namespace game
{
namespace states
{

void GameState::enter(Game& game)
{
	Super::enter(game);
	game.time->setNoLimitFrameRate();
	
#ifdef FLAT_DEBUG
	if (!m_isReloading)
	{
#endif
		setCameraZoom(2.f);
#ifdef FLAT_DEBUG
	}
#endif
	
	getMap().buildNeighborTiles();
	startLevelScript(game);
}

void GameState::execute(Game& game)
{
	Super::execute(game);

#ifdef FLAT_DEBUG
	handleDebugInputs(game);
#endif

	handleGameActionInputs(game);
	updateEntities();
	updateMap();
	updateLevelScript();
}

void GameState::setCanPlaceGhostEntity(flat::lua::UniqueLuaReference<LUA_TFUNCTION>&& canPlaceGhostEntity)
{
	m_canPlaceGhostEntity = std::move(canPlaceGhostEntity);
}

void GameState::setOnGhostEntityPlaced(flat::lua::UniqueLuaReference<LUA_TFUNCTION>&& onGhostEntityPlaced)
{
	m_onGhostEntityPlaced = std::move(onGhostEntityPlaced);
}

bool GameState::canPlaceGhostEntity(map::TileIndex tileIndex) const
{
	FLAT_ASSERT(tileIndex != map::TileIndex::INVALID);
	if (m_canPlaceGhostEntity)
	{
		bool canPlaceEntity = false;
		m_canPlaceGhostEntity.callFunction(
			[tileIndex](lua_State* L)
			{
				map::brush::TilesContainer* tilesContainer = new map::brush::TilesContainer();
				tilesContainer->emplace_back(tileIndex, 1.f);
				map::brush::lua::pushTilesContainer(L, tilesContainer);
			},
			1,
			[&canPlaceEntity](lua_State* L)
			{
				canPlaceEntity = lua_toboolean(L, -1) == 1;
			}
		);
		return canPlaceEntity;
	}
	return true;
}

bool GameState::onGhostEntityPlaced()
{
	if (m_canPlaceGhostEntity)
	{
		bool placeEntity = false;
		m_onGhostEntityPlaced.callFunction(
			[](lua_State* L) {},
			1,
			[&placeEntity](lua_State* L)
			{
				placeEntity = lua_toboolean(L, -1) == 1;
			}
		);
		return placeEntity;
	}
	return true;
}

void GameState::startLevelScript(Game& game)
{
	lua_State* L = game.lua->state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		flat::lua::doFile(L, "data/game/scripts/init.lua");

		flat::lua::loadFile(L, m_mod.getMapPath(game.mapName, "setup.lua"));
		if (!lua_isnil(L, -1))
		{
			luaL_checktype(L, -1, LUA_TFUNCTION);
			m_levelThread.set(L, -1);
			m_levelThread.start(0);
		}

		lua_pop(L, 1);
	}
}

void GameState::updateLevelScript()
{
	FLAT_PROFILE("Update level script");

	if (!m_levelThread.isFinished())
	{
		m_levelThread.update();
	}
}

#ifdef FLAT_DEBUG
void GameState::handleDebugInputs(Game & game)
{
	const auto& keyboard = game.input->keyboard;

	if (keyboard->isJustPressed(K(F6)))
	{
		return reloadToState<MapEditorState>(game);
	}
	else if (keyboard->isJustPressed(K(F7)))
	{
		return reloadToState<GameState>(game);
	}
	else if (m_pauseNextFrame)
	{
		setGamePause(game, true, false);
	}

	std::vector<entity::Entity*>& entitiesToDebug = m_selectedEntities.empty() ? m_entityUpdater.getEntities() : m_selectedEntities;

	if (keyboard->isJustPressed(K(F1)))
	{
		for (entity::Entity* entity : entitiesToDebug)
		{
			entity->setDebug(true);
			entity->setDebugAllComponents(true);
		}
	}
	else if (keyboard->isJustPressed(K(F2)))
	{
		for (entity::Entity* entity : entitiesToDebug)
		{
			entity->setDebug(false);
			entity->setDebugAllComponents(false);
		}
	}
}
#endif

} // states
} // game



