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
	
	Game* game = agent->to<Game>();
	
	std::shared_ptr<const entity::EntityTemplate> entityTemplate = getEntityTemplate(game, "sheep");
	m_sheep = new entity::Entity(entityTemplate, m_luaState);
	flat::geometry::Vector3 position(m_map.getWidth() / 2.f, m_map.getHeight() / 2.f, 0.f);
	m_sheep->setPosition(position);
	m_map.addEntity(m_sheep);
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
			m_sheep->addPointOnPath(clickedTilePosition);
		}
	}
	
	flat::time::Time* time = game->time;
	m_map.updateEntities(time->getTime(), time->getFrameTime());
	
	Super::execute(agent);
}

} // states
} // game



