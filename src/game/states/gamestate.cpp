#include "gamestate.h"
#include "../game.h"
#include "../entity/entity.h"
#include "../entity/entitytemplate.h"

namespace game
{
namespace states
{

void GameState::enter(Game& game)
{
#ifdef FLAT_DEBUG
	m_gamePaused = false;
	m_pauseNextFrame = false;
#endif

	Super::enter(game);
	
	setCameraZoom(2.f);
	
	lua_State* L = game.lua->state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		flat::lua::doFile(L, "data/game/scripts/ui.lua");

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

void GameState::execute(Game& game)
{
	const flat::input::Mouse* mouse = game.input->mouse;
	const flat::input::Keyboard* keyboard = game.input->keyboard;

	if (isMouseOverUi(game))
	{
		clearMouseOverEntity();
	}
	else
	{
		updateMouseOverEntity(game);
		if (!updateSelectionWidget(game))
		{
			if (mouse->isJustReleased(M(LEFT)))
			{
				clearSelection();
			}
		}

		if (mouse->isJustPressed(M(RIGHT)))
		{
			flat::Vector2 clickedTilePosition = getCursorMapPosition(game);
			map::Tile* clickedTile = m_map.getTileIfWalkable(clickedTilePosition.x, clickedTilePosition.y);
			if (clickedTile)
			{
				if (!keyboard->isPressed(K(LSHIFT)))
				{
					for (entity::Entity* entity : m_selectedEntities)
					{
						entity->clearPath();
					}
				}

				for (entity::Entity* entity : m_selectedEntities)
				{
					entity->addPointOnPath(clickedTilePosition);
				}
			}
		}
	}

	if (keyboard->isJustPressed(K(F)))
	{
		moveToFormation(game);
	}

#ifdef FLAT_DEBUG
	if (keyboard->isJustPressed(K(F10)))
	{
		toggleGamePause(game);
	}
	else if (keyboard->isJustPressed(K(F11)))
	{
		setGamePause(game, false);
		m_pauseNextFrame = true;
	}
	else if (m_pauseNextFrame)
	{
		setGamePause(game, true);
		m_pauseNextFrame = false;
	}

	std::vector<entity::Entity*>& entitiesToDebug = m_selectedEntities.empty() ? getMap().getEntities() : m_selectedEntities;

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

	if (keyboard->isJustPressed(K(F3)))
	{
		for (entity::Entity* entity : entitiesToDebug)
		{
			entity->setDebugBreak(true);
		}
		m_pauseNextFrame = true;
	}
#endif

#ifdef FLAT_DEBUG
	if (!isGamePaused())
	{
#endif

	despawnEntities();
	flat::time::Time* time = game.time;
	m_map.updateEntities(time->getTime(), time->getFrameTime());
	m_mapDisplayManager.updateEntities();

#ifdef FLAT_DEBUG
	}
#endif

	if (!m_levelThread.isFinished())
	{
		m_levelThread.update();
	}
	
	Super::execute(game);
}

void GameState::moveToFormation(Game& game)
{
	// 1. compute centroid
	flat::Vector2 centroid(0.f, 0.f);
	for (entity::Entity* entity : m_selectedEntities)
	{
		centroid += flat::Vector2(entity->getPosition());
	}
	centroid /= m_selectedEntities.size();

	// 2. sort by farthest to closest
	std::vector<entity::Entity*> formationEntities = m_selectedEntities;
	std::sort(formationEntities.begin(), formationEntities.end(),
		[centroid](entity::Entity* a, entity::Entity* b)
		{
			//const float da = flat::length2(flat::Vector2(a->getPosition()) - centroid);
			//const float db = flat::length2(flat::Vector2(b->getPosition()) - centroid);
			const float da = std::abs(a->getPosition().x - centroid.x) + std::abs(a->getPosition().y - centroid.y);
			const float db = std::abs(b->getPosition().x - centroid.x) + std::abs(b->getPosition().y - centroid.y);
			return da < db;
		}
	);

	// 3. compute formation positions
	std::vector<flat::Vector2> formationPositions;
	formationPositions.reserve(formationEntities.size());
	const int squareSize = static_cast<int>(std::floor(std::sqrt(formationEntities.size())));
	const float distanceBetweenEntities = 0.7f;
	int row = 0;
	int col = 0;
	for (size_t i = 0; i < formationEntities.size(); ++i)
	{
		flat::Vector2 formationPosition;
		formationPosition.x = centroid.x + (static_cast<float>(col) - static_cast<float>(squareSize - 1) / 2.f) * distanceBetweenEntities;
		formationPosition.y = centroid.y + (static_cast<float>(row) - static_cast<float>(squareSize - 1) / 2.f) * distanceBetweenEntities;
		++col;
		if (col >= squareSize)
		{
			col = 0;
			++row;
		}
		formationPositions.push_back(formationPosition);
	}
	FLAT_ASSERT(formationPositions.size() == formationEntities.size());

	// 4. find closest formation point for each entity and remove point from available positions
	for (entity::Entity* entity : formationEntities)
	{
		// find closest solution
		std::vector<flat::Vector2>::iterator closestPositionIt = formationPositions.end();
		float closestPositionDistance2 = std::numeric_limits<float>::max();
		for (std::vector<flat::Vector2>::iterator it = formationPositions.begin(); it != formationPositions.end(); ++it)
		{
			const float distanceToPosition2 = flat::length2(*it - flat::Vector2(entity->getPosition()));
			if (distanceToPosition2 < closestPositionDistance2)
			{
				closestPositionDistance2 = distanceToPosition2;
				closestPositionIt = it;
			}
		}
		FLAT_ASSERT(closestPositionIt != formationPositions.end());
		entity->clearPath();
		entity->addPointOnPath(*closestPositionIt);
		formationPositions.erase(closestPositionIt);
	}
	FLAT_ASSERT(formationPositions.empty());
}

#ifdef FLAT_DEBUG
void GameState::setGamePause(Game& game, bool pause)
{
	if (m_gamePaused == pause)
	{
		return;
	}

	m_gamePaused = pause;

	flat::time::Time* time = game.time;
	if (pause)
	{
		time->pause();
	}
	else
	{
		time->resume();
	}
}

void GameState::toggleGamePause(Game& game)
{
	setGamePause(game, !m_gamePaused);
#ifdef FLAT_DEBUG
	if (!m_gamePaused)
	{
		for (entity::Entity* entity : getMap().getEntities())
		{
			entity->setDebugBreak(false);
		}
	}
#endif
}
#endif

} // states
} // game



