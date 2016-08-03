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
	m_sheep = new entity::Entity(entityTemplate);
	m_sheep->setPosition(flat::geometry::Vector3(m_map.getWidth() / 2.f, m_map.getHeight() / 2.f, 0.f));
	m_map.addEntity(m_sheep);
}

void GameState::execute(flat::state::Agent* agent)
{
	Game* game = agent->to<Game>();
	
	float currentTime = game->time->getTime();
	float f = currentTime * 0.6f;
	flat::geometry::Vector3 position(m_map.getWidth() / 2.f, m_map.getHeight() / 2.f, 0.f);
	position.x += cos(f) * 2.f;
	position.y += sin(f) * 2.f;
	m_sheep->setPosition(position);
	m_sheep->setHeading(f + M_PI / 2.f);
	m_sheep->update(currentTime);
	
	Super::execute(agent);
}

} // states
} // game



