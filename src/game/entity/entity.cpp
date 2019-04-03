#include "entity.h"
#include "entitytemplate.h"
#include "component/components/behavior/behaviorcomponent.h"
#include "component/components/collision/collisioncomponent.h"
#include "component/components/movement/movementcomponent.h"
#include "component/components/selection/selectioncomponent.h"
#include "../map/map.h"
#include "../map/tile.h"
#include "../map/displaymanager.h"

namespace game
{
namespace entity
{

const flat::render::ProgramSettings* Entity::entityProgramSettings = nullptr;

Entity::Entity(const std::shared_ptr<const EntityTemplate>& entityTemplate, EntityId id) :
	m_behaviorComponent(nullptr),
	m_collisionComponent(nullptr),
	m_movementComponent(nullptr),
	m_sprite(nullptr),
	m_id(id),
	m_heading(0.f),
	m_elevation(0.f),
	m_map(nullptr),
	m_cellIndex(-1),
	m_template(entityTemplate),
	m_canBeSelected(false),
	m_selected(false),
	m_markedForDelete(false),
	m_aabbDirty(false),
	m_aabbCanChange(true)
#ifdef FLAT_DEBUG
	, m_debug(false)
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
	FLAT_ASSERT(m_map == nullptr || getTileIndexFromPosition() != map::TileIndex::INVALID_TILE);
	if (m_map)
	{
		m_cellIndex = m_map->updateEntityPosition(this, m_cellIndex);
		positionChanged(m_position);
	}

	m_aabbDirty = true;
}

void Entity::setXY(const flat::Vector2& xy)
{
	m_position.x = xy.x;
	m_position.y = xy.y;
	FLAT_ASSERT(m_map == nullptr || getTileIndexFromPosition() != map::TileIndex::INVALID_TILE);
	if (m_map)
	{
		m_cellIndex = m_map->updateEntityPosition(this, m_cellIndex);
		positionChanged(m_position);
	}

	m_aabbDirty = true;
}

void Entity::setZ(float z)
{
	m_position.z = z;
	if (m_map)
	{
		positionChanged(m_position);
	}

	m_aabbDirty = true;
}

void Entity::setHeading(float heading, float epsilon)
{
	const float difference = std::abs(flat::angle_clamp_pi(heading - m_heading));
	if (difference >= epsilon)
	{
		m_heading = flat::angle_clamp_0_2pi(heading);
		if (m_map != nullptr)
		{
			headingChanged(m_heading);
		}

		m_aabbDirty = true;
	}
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
	if (m_map != nullptr)
	{
		elevationChanged(m_elevation);
	}

	m_aabbDirty = true;
}

flat::render::BaseSprite& Entity::getSprite()
{
	FLAT_ASSERT(m_sprite != nullptr);
	return *m_sprite;
}

const flat::render::ProgramSettings& Entity::getProgramSettings() const
{
	return getEntityProgramSettings();
}

void Entity::addToMap(map::Map* map)
{
	FLAT_ASSERT(map != nullptr && m_map == nullptr);
	m_map = map;

	updateAABB();

	m_cellIndex = m_map->addEntity(this);
	addedToMap(this, map);
	positionChanged(m_position);
	headingChanged(m_heading);
	elevationChanged(m_elevation);

	updateAABB();

#ifdef FLAT_DEBUG
	checkSpriteAABB();
#endif
}

void Entity::removeFromMap()
{
	FLAT_ASSERT(m_map != nullptr);
	removedFromMap(this);
	m_map->removeEntity(this, m_cellIndex);
	m_map = nullptr;
	m_cellIndex = -1;
}

#ifdef FLAT_DEBUG
void Entity::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	if (m_debug && !m_markedForDelete)
	{
		for (component::Component* component : m_components)
		{
			if (component->getDebug())
			{
				component->debugDraw(debugDisplay);
			}
		}
	}
}
#endif

bool Entity::isBusy() const
{
	for (component::Component* component : m_components)
	{
		if (component->isEnabled() && component->isBusy())
		{
			return true;
		}
	}

	return false;
}

bool Entity::isBusy(component::ComponentFlags componentFlags) const
{
	for (component::Component* component : m_components)
	{
		if (component->isEnabled())
		{
			component::ComponentFlags componentFlag = component->getComponentType().getComponentTypeFlag();
			if ((componentFlag & componentFlags) == componentFlag && component->isBusy())
			{
				return true;
			}
		}
	}

	return false;
}

void Entity::cancelCurrentActions()
{
	for (component::Component* component : m_components)
	{
		if (component->isEnabled())
		{
			component->cancelCurrentAction();
		}
	}
}

void Entity::cancelCurrentActions(component::ComponentFlags componentFlags)
{
	for (component::Component* component : m_components)
	{
		if (component->isEnabled())
		{
			component::ComponentFlags componentFlag = component->getComponentType().getComponentTypeFlag();
			if ((componentFlag & componentFlags) == componentFlag)
			{
				component->cancelCurrentAction();
			}
		}
	}
}

bool Entity::acceptsMoveOrders() const
{
	return m_movementComponent != nullptr;
}

void Entity::moveTo(const flat::Vector2& point, Entity* interactionEntity)
{
	FLAT_ASSERT(m_movementComponent != nullptr);
	m_movementComponent->moveTo(point, interactionEntity);
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

void Entity::setSelected(bool selected)
{
	FLAT_ASSERT(selected != m_selected);
	m_selected = selected;

	component::selection::SelectionComponent* selectionComponent = getComponent<component::selection::SelectionComponent>();
	if (selectionComponent != nullptr)
	{
		if (selected)
		{
			selectionComponent->selected();
		}
		else
		{
			selectionComponent->deselected();
		}
	}
}

#ifdef FLAT_DEBUG
void Entity::setDebugAllComponents(bool debug)
{
	for (component::Component* component : m_components)
	{
		component->setDebug(debug);
	}
}

component::ComponentFlags Entity::getDebuggedComponentFlags() const
{
	component::ComponentFlags debuggedComponentFlags = 0;
	for (component::Component* component : m_components)
	{
		if (component->getDebug())
		{
			debuggedComponentFlags |= component->getComponentType().getComponentTypeFlag();
		}
	}
	return debuggedComponentFlags;
}

void Entity::setDebuggedComponentFlags(component::ComponentFlags debuggedComponentFlags)
{
	for (component::Component* component : m_components)
	{
		component::ComponentFlags componentFlag = component->getComponentType().getComponentTypeFlag();
		component->setDebug((debuggedComponentFlags & componentFlag) == componentFlag);
	}
}
#endif

map::TileIndex Entity::getTileIndexFromPosition()
{
	FLAT_ASSERT(m_map != nullptr);
	map::TileIndex tileIndex = m_map->getTileIndex(m_position.x, m_position.y);
	FLAT_ASSERT_MSG(tileIndex != map::TileIndex::INVALID_TILE, "Trying to get a tile that does not exist at position (%f, %f)", m_position.x, m_position.y);
	return tileIndex;
}

void Entity::updateAABB()
{
	if (m_aabbCanChange)
	{
		if (m_collisionComponent != nullptr)
		{
			m_collisionComponent->getAABB(m_worldSpaceAABB);
		}
		else
		{
			m_worldSpaceAABB.min = m_position;
			m_worldSpaceAABB.max = m_position;
		}
	}

	if (m_sprite != nullptr)
	{
		m_sprite->getAABB(m_spriteAABB);
	}

#ifdef FLAT_DEBUG
	checkSpriteAABB();
#endif
}

void Entity::updateAABBIfDirty()
{
	if (m_aabbDirty)
	{
		m_aabbDirty = false;

		updateAABB();

		if (m_map)
		{
			m_map->getDisplayManager().updateEntity(this);
		}
	}

#ifdef FLAT_DEBUG
	checkSpriteAABB();
#endif
}

bool Entity::canInteract() const
{
	return m_behaviorComponent != nullptr;
}

void Entity::enterState(const char* stateName)
{
	FLAT_ASSERT(m_behaviorComponent != nullptr);
	m_behaviorComponent->enterState(stateName);
}

void Entity::setInteractionIfCompatible(const char* stateName, entity::Entity* interactionEntity)
{
	FLAT_ASSERT(m_behaviorComponent != nullptr);
	FLAT_ASSERT(interactionEntity != nullptr);
	m_behaviorComponent->setInteractionIfCompatible(stateName, interactionEntity);
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
	m_collisionComponent = findComponent<component::collision::CollisionComponent>();
	m_movementComponent = findComponent<component::movement::MovementComponent>();
}

void Entity::initComponents()
{
	for (component::Component* component : m_components)
	{
		FLAT_ASSERT(component->isEnabled());
		component->init();
	}

#ifdef FLAT_DEBUG
	if (isBusy())
	{
		for (component::Component* component : m_components)
		{
			if (component->isBusy())
			{
				std::cerr << "Component " << component->getComponentType().getConfigName() << " is busy" << std::endl;
			}
		}
		FLAT_ASSERT_MSG(false, "Entity %s should not be busy right after components init", getTemplateName().c_str());
	}
#endif
}

void Entity::deinitComponents()
{
	for (component::Component* component : m_components)
	{
		component->deinit();
	}
}

const component::Component* Entity::findComponent(component::ComponentFlags componentFlag) const
{
	for (const component::Component* component : m_components)
	{
		if (component->getComponentType().getComponentTypeFlag() == componentFlag)
			return component;
	}
	return nullptr;
}

component::Component* Entity::findComponent(component::ComponentFlags componentFlag)
{
	for (component::Component* component : m_components)
	{
		if (component->getComponentType().getComponentTypeFlag() == componentFlag)
			return component;
	}
	return nullptr;
}

#ifdef FLAT_DEBUG
void Entity::checkSpriteAABB()
{
	// ensure the sprite AABB is up to date
	if (hasSprite())
	{
		flat::AABB2 spriteAABB;
		getSprite().getAABB(spriteAABB);
		FLAT_ASSERT(spriteAABB == getAABB());
	}
}

#endif

} // entity
} // game


