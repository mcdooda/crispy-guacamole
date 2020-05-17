#include "gamestate.h"
#include "../game.h"
#include "../entity/entity.h"
#include "../entity/entitytemplate.h"
#include "map/pathfinder/path.h"
#include "map/pathfinder/lua/path.h"
#include "map/brush/lua/brush.h"
#include "map/brush/lua/tilescontainer.h"
#include "lua/table.h"

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
	updateMap(game);
	updateLevelScript();
}

void GameState::setGhostEntitiesPositions(flat::lua::UniqueLuaReference<LUA_TFUNCTION>&& ghostEntitiesPositions)
{
	m_getGhostEntityPositions = std::move(ghostEntitiesPositions);
}

void GameState::setOnGhostEntityPlaced(flat::lua::UniqueLuaReference<LUA_TFUNCTION>&& onGhostEntityPlaced)
{
	m_onGhostEntityPlaced = std::move(onGhostEntityPlaced);
}

std::vector<flat::Vector2> GameState::getGhostEntityPositions(const flat::Vector2& cursorPosition, map::TileIndex tileIndex) const
{
	FLAT_ASSERT(tileIndex != map::TileIndex::INVALID_TILE);
	std::vector<flat::Vector2> points;
	if (m_getGhostEntityPositions)
	{
		m_getGhostEntityPositions.callFunction(
			[&cursorPosition, tileIndex](lua_State* L)
			{
				flat::lua::pushVector2(L, cursorPosition);
				map::brush::TilesContainer* tilesContainer = new map::brush::TilesContainer();
				tilesContainer->push_back({tileIndex, 1.f});
				map::brush::lua::pushTilesContainer(L, tilesContainer);
			},
			1,
			[&points](lua_State* L)
			{
				if (!lua_isnoneornil(L, 1))
				{
					points = flat::lua::table::getArray<flat::Vector2>(L, 1);
				}
			}
		);
		return points;
	}
	return points;
}

bool GameState::onGhostEntityPlaced(map::TileIndex tileIndex, bool& continueAction)
{
	if (m_getGhostEntityPositions)
	{
		bool placeEntity = false;
		m_onGhostEntityPlaced.callFunction(
			[tileIndex, continueAction](lua_State* L)
			{
				map::brush::TilesContainer* tilesContainer = new map::brush::TilesContainer();
				tilesContainer->emplace_back(tileIndex, 1.f);
				map::brush::lua::pushTilesContainer(L, tilesContainer);
				lua_pushboolean(L, continueAction);
			},
			2,
			[&placeEntity, &continueAction](lua_State* L)
			{
				placeEntity = lua_toboolean(L, -2) == 1;
				continueAction |= lua_toboolean(L, -1) == 1;
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

		flat::lua::loadFile(L, game.mod.getMapFilePath(game.mapName, "setup.lua"));
		if (!lua_isnil(L, -1))
		{
			luaL_checktype(L, -1, LUA_TFUNCTION);
			m_levelThread.set(L, -1);
			m_levelThread.resume(0, 0);
		}

		lua_pop(L, 1);
	}
}

void GameState::updateLevelScript()
{
	FLAT_PROFILE("Update level script");

	if (!m_levelThread.isFinished())
	{
		m_levelThread.resume(0, 0);
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



