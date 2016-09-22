#include "gamestate.h"
#include "../game.h"
#include "../entity/entity.h"
#include "../entity/entitytemplate.h"

#include "../entity/component/behaviorcomponent.h"
#include "../entity/component/movementcomponent.h"
#include "../entity/component/spritecomponent.h"

namespace game
{
namespace states
{

void GameState::enter(flat::state::Agent* agent)
{
	Super::enter(agent);
	
	setCameraZoom(2.f);
	
	flat::lua::doFile(m_luaState, "data/game/scripts/ui.lua");
	
	Game* game = agent->to<Game>();
	
	static const char* entityTemplates[] = {
		"sheep",
		//"devil",
		//"larva",
		nullptr
	};
	
	for (int j = 0; entityTemplates[j]; ++j)
	{
		std::shared_ptr<const entity::EntityTemplate> entityTemplate = getEntityTemplate(game, entityTemplates[j]);
		for (int i = 0; i < 10; ++i)
		{
			flat::Vector3 position;
			int tileX, tileY;
			do
			{
				position.x = m_map.getWidth() / 2.f + game->random->nextFloat(-1.f, 1.f);
				position.y = m_map.getHeight() / 2.f + game->random->nextFloat(-1.f, 1.f);
				tileX = static_cast<int>(std::round(position.x));
				tileY = static_cast<int>(std::round(position.y));
			}
			while (m_map.getTileIfWalkable(tileX, tileY) == nullptr);

			spawnEntityAtPosition(entityTemplate, position);
		}
	}
}

void GameState::execute(flat::state::Agent* agent)
{
	Game* game = agent->to<Game>();
	
	if (game->input->mouse->isJustPressed(M(LEFT)))
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
	
	flat::time::Time* time = game->time;
	m_map.updateEntities(time->getTime(), time->getFrameTime());
	
	Super::execute(agent);
}

} // states
} // game



