#include "entity.h"
#include "entitytemplate.h"
#include "component/components/behavior/behaviorcomponent.h"
#include "component/components/movement/movementcomponent.h"
#include "component/components/sprite/spritecomponent.h"
#include "component/components/texture/texturecomponent.h"
#include "../map/map.h"
#include "../map/tile.h"

namespace game
{
namespace entity
{

Entity::Entity(const std::shared_ptr<const EntityTemplate>& entityTemplate, EntityId id) :
	m_behaviorComponent(nullptr),
	m_movementComponent(nullptr),
	m_spriteComponent(nullptr),
	m_textureComponent(nullptr),
	m_id(id),
	m_heading(0.f),
	m_elevation(0.f),
	m_map(nullptr),
	m_tile(nullptr),
	m_template(entityTemplate),
	m_selected(false)
#ifdef FLAT_DEBUG
	, m_debug(false)
	, m_debugBreak(false)
#endif
{

}

Entity::~Entity()
{
	m_id = EntityIdFactory::InvalidId; // writes InvalidId into the entity pool to make handles to this entity invalid
	//FLAT_ASSERT(m_map == nullptr);
	FLAT_ASSERT(m_components.empty()); // all components must have been destroyed before
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
	heading = fmodf(heading, flat::PI2);
	if (heading < 0.f)
		heading += flat::PI2;
	
	m_heading = heading;
	headingChanged(m_heading);
}

void Entity::setElevation(float elevation)
{
	// keep elevation in [-pi, pi) range
	elevation = fmodf(elevation, flat::PI2);
	if (elevation < -flat::PI)
		elevation += flat::PI2;

	else if (elevation > flat::PI)
		elevation -= flat::PI2;

	m_elevation = elevation;
	elevationChanged(m_elevation);
}

const flat::render::Sprite& Entity::getSprite() const
{
	FLAT_ASSERT(m_spriteComponent != nullptr || m_textureComponent != nullptr);

	if (m_spriteComponent != nullptr)
		return m_spriteComponent->getSprite();

	else
		return m_textureComponent->getSprite();
}

void Entity::onAddedToMap(map::Map* map)
{
	FLAT_ASSERT(map && !m_map && !m_tile);
	m_map = map;
	m_tile = getTileFromPosition();
	m_tile->addEntity(this);
	addedToMap(map);
	positionChanged(m_position);
	headingChanged(m_heading);
	elevationChanged(m_elevation);
}

void Entity::onRemovedFromMap()
{
	FLAT_ASSERT(m_map && m_tile);
	m_tile->removeEntity(this);
	m_map = nullptr;
	m_tile = nullptr;
}

void Entity::update(float currentTime, float elapsedTime)
{
#ifdef FLAT_DEBUG
	if (m_debugBreak)
	{
		FLAT_BREAK();
	}
#endif
	for (component::Component* component : m_components)
	{
		if (component->isEnabled())
		{
			component->update(currentTime, elapsedTime);
		}
	}
}

#ifdef FLAT_DEBUG
void Entity::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	if (m_debug)
	{
		for (component::Component* component : m_components)
		{
			component->debugDraw(debugDisplay);
		}
	}
}
#endif

bool Entity::isBusy() const
{
	for (component::Component* component : m_components)
	{
		if (component->isEnabled() && component->isBusy())
			return true;
	}
	
	return false;
}

void Entity::addPointOnPath(const flat::Vector2& point)
{
	FLAT_ASSERT(m_movementComponent != nullptr);
	m_movementComponent->addPointOnPath(point);
}

void Entity::clearPath()
{
	FLAT_ASSERT(m_movementComponent != nullptr);
	m_movementComponent->clearPath();
}

const std::string& Entity::getTemplateName() const
{
	return m_template->getName();
}

bool Entity::canBeSelected() const
{
	return m_movementComponent != nullptr; // TODO: find a more generic method
}

void Entity::setSelected(bool selected)
{
	FLAT_ASSERT(selected != m_selected); // we want to avoid triggering slots again if the selected state did not change
	m_selected = selected;
	if (selected)
	{
		this->selected();
	}
	else
	{
		deselected();
	}
}

map::Tile* Entity::getTileFromPosition()
{
	FLAT_ASSERT(m_map != nullptr);
	map::Tile* tile = m_map->getTileIfExists(m_position.x, m_position.y);
	FLAT_ASSERT_MSG(tile != nullptr, "Trying to get a tile that does not exist");
	return tile;
}

void Entity::enterState(const char* stateName)
{
	FLAT_ASSERT(m_behaviorComponent != nullptr);
	m_behaviorComponent->enterState(stateName);
}

void Entity::addComponent(component::Component* component)
{
	FLAT_ASSERT(component != nullptr);
	component->setOwner(this);
	m_components.push_back(component);
}

void Entity::cacheComponents()
{
	m_behaviorComponent = findComponent<component::behavior::BehaviorComponent>();
	m_movementComponent = findComponent<component::movement::MovementComponent>();
	m_spriteComponent   = findComponent<component::sprite::SpriteComponent>();
	m_textureComponent  = findComponent<component::texture::TextureComponent>();
}

void Entity::initComponents()
{
	for (component::Component* component : m_components)
	{
		component->init();
	}
}

} // entity
} // game


