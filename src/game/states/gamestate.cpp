#include "gamestate.h"
#include "../game.h"
#include "../entity/entity.h"
#include "../entity/entitytemplate.h"

namespace game
{
namespace states
{

void GameState::enter(flat::state::Agent* agent)
{
	Super::enter(agent);
	
	setCameraZoom(2.f);
	
	flat::lua::doFile(m_luaState, "data/game/scripts/ui.lua");
	m_ui->fullLayout();
	
	Game* game = agent->to<Game>();
	
	std::shared_ptr<const entity::EntityTemplate> entityTemplate = getEntityTemplate(game, "sheep");
	
	for (int i = 0; i < 1; ++i)
	{
		entity::Entity* sheep = new entity::Entity(entityTemplate, m_luaState);
		float rx = game->random->nextFloat(-0.1f, 0.1f);
		float ry = game->random->nextFloat(-0.1f, 0.1f);
		flat::geometry::Vector3 position(m_map.getWidth() / 2.f + rx, m_map.getHeight() / 2.f + ry, 0.f);
		sheep->setPosition(position);
		m_map.addEntity(sheep);
		m_sheeps.push_back(sheep);
	}
}

void GameState::execute(flat::state::Agent* agent)
{
	Game* game = agent->to<Game>();
	
	if (game->input->mouse->isJustPressed(M(LEFT)))
	{
		flat::geometry::Vector2 clickedTilePosition = getCursorMapPosition(game);
		int x = round(clickedTilePosition.x);
		int y = round(clickedTilePosition.y);
		map::Tile* clickedTile = m_map.getTileIfWalkable(x, y);
		if (clickedTile)
		{
			for (entity::Entity* sheep : m_sheeps)
			{
				sheep->addPointOnPath(clickedTilePosition);
			}
		}
	}
	
	flat::time::Time* time = game->time;
	m_map.updateEntities(time->getTime(), time->getFrameTime());
	
	Super::execute(agent);
}

} // states
} // game



