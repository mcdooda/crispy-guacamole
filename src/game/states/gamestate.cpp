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
	
	Game* game = agent->to<Game>();
	
	std::shared_ptr<const entity::EntityTemplate> entityTemplate = getEntityTemplate(game, "sheep");
	m_sheep = new entity::Entity(entityTemplate, m_luaState);
	flat::geometry::Vector3 position(m_map.getWidth() / 2.f, m_map.getHeight() / 2.f, 0.f);
	m_sheep->setPosition(position);
	m_sheep->addPointOnPath(flat::geometry::Vector2(position.x + 2.f, position.y + 2.f));
	m_sheep->addPointOnPath(flat::geometry::Vector2(position.x - 2.f, position.y - 2.f));
	m_sheep->addPointOnPath(flat::geometry::Vector2(position.x - 2.f, position.y + 2.f));
	m_sheep->addPointOnPath(flat::geometry::Vector2(position.x + 2.f, position.y - 2.f));
	m_map.addEntity(m_sheep);
}

void GameState::execute(flat::state::Agent* agent)
{
	Game* game = agent->to<Game>();
	
	flat::time::Time* time = game->time;
	m_sheep->update(time->getTime(), time->getFrameTime());
	
	Super::execute(agent);
}

} // states
} // game



