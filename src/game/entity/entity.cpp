#include "entity.h"
#include "entitytemplate.h"
#include "../map/map.h"
#include "../map/tile.h"

namespace game
{
namespace entity
{

Entity::Entity(std::shared_ptr<const EntityTemplate> entityTemplate, lua_State* L) :
	m_heading(0.f),
	m_map(nullptr),
	m_tile(nullptr),
	m_template(entityTemplate)
{
	registerComponent(m_behaviorComponent);
	registerComponent(m_movementComponent);
	registerComponent(m_spriteComponent);
}

Entity::~Entity()
{
	
}

void Entity::setPosition(const flat::geometry::Vector3& position)
{
	m_position = position;
	if (m_map)
	{
		map::Tile* newTile = getTileFromPosition();
		if (m_tile && m_tile != newTile)
		{
			m_tile->removeEntity(this);
			newTile->addEntity(this);
		}
		m_tile = newTile;
		m_spriteComponent.updatePosition();
	}
}

void Entity::setHeading(float heading)
{
	// keep heading in [0, 2pi) range
	heading = fmodf(heading, M_PI * 2.f);
	if (heading < 0.f)
		heading += M_PI * 2.f;
	
	m_heading = heading;
	m_spriteComponent.updateHeading();
}

void Entity::draw(const flat::util::RenderSettings& renderSettings, const flat::geometry::Matrix4& viewMatrix) const
{
	m_spriteComponent.draw(renderSettings, viewMatrix);
}

void Entity::onAddedToMap(map::Map* map)
{
	FLAT_ASSERT(map && !m_map && !m_tile);
	m_map = map;
	m_tile = getTileFromPosition();
	m_tile->addEntity(this);
	m_spriteComponent.updatePosition();
	enterState("init");
}

void Entity::onRemovedFromMap()
{
	FLAT_ASSERT(m_map && m_tile);
	m_map = nullptr;
	m_tile = nullptr;
}

void Entity::update(float currentTime, float elapsedTime)
{
	for (component::Component* component : m_components)
	{
		component->update(currentTime, elapsedTime);
	}
}

map::Tile* Entity::getTileFromPosition()
{
	FLAT_ASSERT(m_map);
	int tileX = floor(m_position.x + 0.5f);
	int tileY = floor(m_position.y + 0.5f);
	return m_map->getTileIfExists(tileX, tileY);
}

void Entity::enterState(const char* stateName)
{
	m_behaviorComponent.enterState(stateName);
}

void Entity::setSpriteColumn(int column)
{
	m_spriteComponent.setColumn(column);
}

void Entity::registerComponent(component::Component& component)
{
	component.setOwner(this);
	m_components.push_back(&component);
}

} // entity
} // game


