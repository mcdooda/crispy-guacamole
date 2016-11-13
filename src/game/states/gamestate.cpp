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
			for (entity::Entity* entity : m_selectedEntities)
			{
				entity->addPointOnPath(clickedTilePosition);
			}
		}
	}

#ifdef FLAT_DEBUG
	if (keyboard->isJustPressed(K(F1)))
	{
		for (entity::Entity* entity : m_selectedEntities)
		{
			entity->enableDebug(true);
		}
	}
	else if (keyboard->isJustPressed(K(F2)))
	{
		for (entity::Entity* entity : m_selectedEntities)
		{
			entity->enableDebug(false);
		}
	}
#endif

	despawnEntities();
	flat::time::Time* time = game.time;
	m_map.updateEntities(time->getTime(), time->getFrameTime());
	
	Super::execute(game);
}

} // states
} // game



