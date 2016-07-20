#include "gamestate.h"
#include "../game.h"
#include "../map/entity.h"
#include "../map/entitytemplate.h"

namespace game
{
namespace states
{

void GameState::enter(flat::state::Agent* agent)
{
	Super::enter(agent);
	
	Game* game = agent->to<Game>();
	
	map::EntityTemplate* entityTemplatePtr = new map::EntityTemplate();
	entityTemplatePtr->setSpriteOrigin(flat::geometry::Vector2(15.f, 27.f));
	entityTemplatePtr->setAtlas(game->video->getTexture("data/game/units/soldier/atlas.png"));
	entityTemplatePtr->setRadius(0.3f);
	
	std::shared_ptr<const map::EntityTemplate> entityTemplate(entityTemplatePtr);
	m_soldier = new map::Entity(entityTemplate);
	m_soldier->setPosition(flat::geometry::Vector3(m_map.getWidth() / 2.f, m_map.getHeight() / 2.f, 0.f));
	m_map.addEntity(m_soldier);
}

void GameState::execute(flat::state::Agent* agent)
{
	Game* game = agent->to<Game>();
	
	float f = game->time->getTime() * 3.f;
	flat::geometry::Vector3 position(m_map.getWidth() / 2.f, m_map.getHeight() / 2.f, 0.f);
	position.x += cos(f) * 2.f;
	position.y += sin(f) * 2.f;
	m_soldier->setPosition(position);
	m_soldier->setHeading(f);
	
	Super::execute(agent);
}

} // states
} // game



