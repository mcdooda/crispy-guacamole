#include "entity.h"
#include "entitytemplate.h"
#include "component/behaviorcomponent.h"
#include "component/movementcomponent.h"
#include "component/spritecomponent.h"
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
	m_behaviorComponent = new component::BehaviorComponent();
	addComponent(m_behaviorComponent);
	m_movementComponent = new component::MovementComponent();
	addComponent(m_movementComponent);
	m_spriteComponent = new component::SpriteComponent();
	addComponent(m_spriteComponent);
}

Entity::~Entity()
{
	destroyComponents();
}

void Entity::setPosition(const flat::Vector3& position)
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
		positionChanged(m_position);
	}
}

void Entity::setXY(const flat::Vector2& xy)
{
	m_position.x = xy.x;
	m_position.y = xy.y;
	if (m_map)
	{
		map::Tile* newTile = getTileFromPosition();
		if (m_tile && m_tile != newTile)
		{
			m_tile->removeEntity(this);
			newTile->addEntity(this);
		}
		m_tile = newTile;
		positionChanged(m_position);
	}
}

void Entity::setZ(float z)
{
	m_position.z = z;
	if (m_map)
	{
		positionChanged(m_position);
	}
}

void Entity::setHeading(float heading)
{
	// keep heading in [0, 2pi) range
	heading = fmodf(heading, M_PI * 2.f);
	if (heading < 0.f)
		heading += M_PI * 2.f;
	
	m_heading = heading;
	headingChanged(m_heading);
}

void Entity::draw(const flat::util::RenderSettings& renderSettings, const flat::Matrix4& viewMatrix) const
{
	m_spriteComponent->draw(renderSettings, viewMatrix);
}

void Entity::onAddedToMap(map::Map* map)
{
	FLAT_ASSERT(map && !m_map && !m_tile);
	m_map = map;
	m_tile = getTileFromPosition();
	m_tile->addEntity(this);
	addedToMap(map);
	positionChanged(m_position);
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

bool Entity::isBusy() const
{
	for (component::Component* component : m_components)
	{
		if (component->isBusy())
			return true;
	}
	
	return false;
}

void Entity::addPointOnPath(const flat::Vector2& point)
{
	m_movementComponent->addPointOnPath(point);
}

map::Tile* Entity::getTileFromPosition()
{
	FLAT_ASSERT(m_map);
	int tileX = floor(m_position.x + 0.5f);
	int tileY = floor(m_position.y + 0.5f);
	map::Tile* tile = m_map->getTileIfExists(tileX, tileY);
	FLAT_ASSERT_MSG(tile, "Trying to get a tile that does not exist");
	return tile;
}

void Entity::destroyComponents()
{
	for (component::Component*& component : m_components)
	{
		FLAT_DELETE(component);
	}
	m_components.clear();
}

void Entity::enterState(const char* stateName)
{
	m_behaviorComponent->enterState(stateName);
}

bool Entity::playAnimation(const char* animationName, int numLoops)
{
	const EntityTemplate* entityTemplatePtr = m_template.get();
	FLAT_ASSERT(entityTemplatePtr);
	const sprite::Description& spriteDescription = entityTemplatePtr->getSpriteDescription();
	if (const sprite::AnimationDescription* animationDescription = spriteDescription.getAnimationDescription(animationName))
	{
		m_spriteComponent->playAnimation(*animationDescription, numLoops);
		return true;
	}
	return false;
}

void Entity::addComponent(component::Component* component)
{
	FLAT_ASSERT(component);
	component->setOwner(this);
	m_components.push_back(component);
}

} // entity
} // game


