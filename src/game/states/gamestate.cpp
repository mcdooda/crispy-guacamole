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
	
	flat::lua::doFile(m_luaState, "data/game/scripts/ui.lua");
}

void GameState::execute(Game& game)
{
	const flat::input::Mouse* mouse = game.input->mouse;
	const flat::input::Keyboard* keyboard = game.input->keyboard;

	if (!m_ui->isMouseOver() || isSelecting())
	{
		if (!updateSelectionWidget(game))
		{
			if (mouse->isJustReleased(M(LEFT)))
			{
				clearSelection(game);
			}
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

	std::vector<entity::Entity*>& entitiesToDebug = m_selectedEntities.empty() ? m_entities : m_selectedEntities;
	if (keyboard->isJustPressed(K(F1)))
	{
		for (entity::Entity* entity : entitiesToDebug)
		{
			entity->enableDebug(true);
		}
	}
	else if (keyboard->isJustPressed(K(F2)))
	{
		for (entity::Entity* entity : entitiesToDebug)
		{
			entity->enableDebug(false);
		}
	}
#endif

#ifdef FLAT_DEBUG
	if (!isGamePaused())
	{
#endif

	despawnEntities();
	flat::time::Time* time = game.time;
	m_map.updateEntities(time->getTime(), time->getFrameTime());

#ifdef FLAT_DEBUG
	}
#endif
	
	Super::execute(game);
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
}
#endif

} // states
} // game



