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
	if (game.input->mouse->isJustPressed(M(LEFT)))
	{
		flat::Vector2 clickedTilePosition = getCursorMapPosition(game);
		map::Tile* clickedTile = m_map.getTileIfWalkable(clickedTilePosition.x, clickedTilePosition.y);
		if (clickedTile)
		{
			for (entity::Entity* entity : m_entities)
			{
				entity->addPointOnPath(clickedTilePosition);
			}
		}
	}
	
	flat::time::Time* time = game.time;
	m_map.updateEntities(time->getTime(), time->getFrameTime());
	
	Super::execute(game);
}

} // states
} // game



