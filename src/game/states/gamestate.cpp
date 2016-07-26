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
	entityTemplatePtr->setSpriteOrigin(flat::geometry::Vector2(12.f, 20.f));
	entityTemplatePtr->setAtlas(game->video->getTexture("data/game/units/sheep/atlas.png"));
	entityTemplatePtr->setAtlasWidth(3);
	entityTemplatePtr->setAtlasHeight(1);
	entityTemplatePtr->setAnimationFrameDuration(0.12f);
	entityTemplatePtr->setRadius(0.3f);
	
	std::shared_ptr<const map::EntityTemplate> entityTemplate(entityTemplatePtr);
	m_sheep = new map::Entity(entityTemplate);
	m_sheep->setPosition(flat::geometry::Vector3(m_map.getWidth() / 2.f, m_map.getHeight() / 2.f, 0.f));
	m_map.addEntity(m_sheep);
}

void GameState::execute(flat::state::Agent* agent)
{
	Game* game = agent->to<Game>();
	
	float currentTime = game->time->getTime();
	float f = currentTime * 1.5f;
	flat::geometry::Vector3 position(m_map.getWidth() / 2.f, m_map.getHeight() / 2.f, 0.f);
	position.x += cos(f) * 2.f;
	position.y += sin(f) * 2.f;
	m_sheep->setPosition(position);
	m_sheep->setHeading(f);
	m_sheep->update(currentTime);
	
	Super::execute(agent);
}

} // states
} // game



