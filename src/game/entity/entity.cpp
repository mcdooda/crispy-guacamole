#include "entity/entity.h"
#include "entity/entitytemplate.h"
#include "entity/component/components/behavior/behaviorcomponent.h"
#include "entity/component/components/collision/collisioncomponent.h"
#include "entity/component/components/movement/movementcomponent.h"
#include "entity/component/components/selection/selectioncomponent.h"
#include "entity/component/components/sprite/spritecomponent.h"
#include "entity/component/components/projectile/projectilecomponent.h"
#include "entity/component/components/prop/propcomponent.h"

#include "map/map.h"
#include "map/displaymanager.h"
#include "map/tile.h"

namespace game
{
namespace entity
{

const flat::render::ProgramSettings* Entity::entityProgramSettings = nullptr;

Entity::Entity(const std::shared_ptr<const EntityTemplate>& entityTemplate, EntityId id) :
	m_behaviorComponent(nullptr),
	m_collisionComponent(nullptr),
	m_movementComponent(nullptr),
	m_spriteComponent(nullptr),
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
	m_aabbCanChange(true),
	m_isGhost(false)
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
#ifdef FLAT_DEBUG
	if (m_map != nullptr)
	{
		checkValidPosition(position);
	}
#endif

	m_position = position;
	if (m_map)
	{
		m_cellIndex = m_map->updateEntityPosition(this, m_cellIndex);
		positionChanged(m_position);
	}

	m_aabbDirty = true;
}

void Entity::setPosition2d(const flat::Vector2& position2d)
{
	setPosition(flat::Vector3(position2d.x, position2d.y, m_position.z));
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

void Entity::setPositionSweep(const flat::Vector3& newPosition)
{
	if (newPosition == m_position)
	{
		return;
	}

	if (!hasComponent<component::collision::CollisionComponent>())
	{
		setPosition(newPosition);
		return;
	}

	// check is the new position is navigable and backtrack if necessary
	// this is necessary to avoid penetrating with props when interacting with them or when trying to move to a non navigable tile

	const flat::Vector2 position2d(m_position);
	FLAT_ASSERT(m_map != nullptr);

	flat::Vector2 newPosition2d(newPosition);

	const map::TileIndex tileIndex = m_map->getTileIndex(newPosition2d);
	FLAT_ASSERT(map::isValidTile(tileIndex));
	const float tileZ = m_map->getTileZ(tileIndex);

	bool isFalling = false;
	if (m_movementComponent != nullptr)
	{
		isFalling = !m_movementComponent->isTouchingGround();
	}

	bool shouldSweepVertically = tileZ - MIN_Z_EPSILON > newPosition.z && isFalling;

	bool shouldSweepHorizontally = false;
	if (!shouldSweepVertically)
	{
		const map::Navigability navigabilityMask = EntityHelper::getNavigabilityMask(this);
		shouldSweepHorizontally = !m_map->isTileNavigable(tileIndex, navigabilityMask);
	}

	if (shouldSweepVertically)
	{
		setPosition(flat::Vector3(newPosition.x, newPosition.y, tileZ - MIN_Z_EPSILON));
	}
	else if (shouldSweepHorizontally)
	{
		// check intersection between current segment and tile and backtrack there, let the collision do the rest later
		const flat::AABB3& tileAABB = m_map->getTileAABB(tileIndex);
		flat::Vector2 intersection = position2d;
		if (position2d.x < newPosition2d.x)
		{
			if (!flat::geometry::intersection::twoLineSegments(
				position2d, newPosition2d,
				flat::Vector2(tileAABB.min), flat::Vector2(tileAABB.min.x, tileAABB.max.y),
				intersection))
			{
				if (position2d.y > newPosition2d.y)
				{
					flat::geometry::intersection::twoLineSegments(
						position2d, newPosition2d,
						flat::Vector2(tileAABB.min.x, tileAABB.max.y), flat::Vector2(tileAABB.max),
						intersection);
				}
				else
				{
					flat::geometry::intersection::twoLineSegments(
						position2d, newPosition2d,
						flat::Vector2(tileAABB.min), flat::Vector2(tileAABB.max.x, tileAABB.min.y),
						intersection);
				}
			}
		}
		else
		{
			if (!flat::geometry::intersection::twoLineSegments(
				position2d, newPosition2d,
				flat::Vector2(tileAABB.max.x, tileAABB.min.y), flat::Vector2(tileAABB.max),
				intersection))
			{
				if (position2d.y > newPosition2d.y)
				{
					flat::geometry::intersection::twoLineSegments(
						position2d, newPosition2d,
						flat::Vector2(tileAABB.min.x, tileAABB.max.y), flat::Vector2(tileAABB.max),
						intersection);
				}
				else
				{
					flat::geometry::intersection::twoLineSegments(
						position2d, newPosition2d,
						flat::Vector2(tileAABB.min), flat::Vector2(tileAABB.max.x, tileAABB.min.y),
						intersection);
				}
			}
		}

		newPosition2d.x = intersection.x;
		newPosition2d.y = intersection.y;

		// move a bit backwards to avoid infinite recursion
		newPosition2d -= flat::normalize(newPosition2d - position2d) * 0.001f;

		setPositionSweep(flat::Vector3(newPosition2d, newPosition.z));
	}
	else
	{
		if (newPosition.z > tileZ - MIN_Z_EPSILON)
		{
			setPosition(newPosition);
		}
		else
		{
			// well, still sweep vertically
			setPosition(flat::Vector3(newPosition.x, newPosition.y, tileZ - MIN_Z_EPSILON));
		}
	}
}

void Entity::setPosition2dSweep(const flat::Vector2& position2d)
{
	setPositionSweep(flat::Vector3(position2d.x, position2d.y, m_position.z));
}

void Entity::computeTransform(flat::Matrix4& transform) const
{
	transform = flat::Matrix4();
	flat::translateBy(transform, m_position);
	flat::rotateZBy(transform, m_heading);
	flat::rotateYBy(transform, -m_elevation);
}

void Entity::setHeading(float heading, float epsilon)
{
	const float difference = std::abs(flat::angle_clamp_pi(heading - m_heading));
	if (difference >= epsilon)
	{
		const float previousHeading = m_heading;
		m_heading = flat::angle_clamp_0_2pi(heading);
		if (m_map != nullptr)
		{
			headingChanged(m_heading, previousHeading);
		}

		m_aabbDirty = true;
	}
}

void Entity::setElevation(float elevation)
{
	// keep elevation in [-pi, pi) range
	elevation = fmodf(elevation, flat::PI_2);
	if (elevation < -flat::PI)
		elevation += flat::PI_2;

	else if (elevation > flat::PI)
		elevation -= flat::PI_2;

	m_elevation = elevation;
	if (m_map != nullptr)
	{
		elevationChanged(m_elevation);
	}

	m_aabbDirty = true;
}

flat::render::BaseSprite* Entity::getSprite()
{
	FLAT_ASSERT(m_sprite != nullptr);
	return m_sprite;
}

void Entity::pushAttachedSprites(std::vector<const MapObject*>& objects) const
{
	if (m_spriteComponent != nullptr)
	{
		m_spriteComponent->pushAttachedSprites(objects);
	}
}

const flat::render::ProgramSettings& Entity::getProgramSettings() const
{
	return getEntityProgramSettings();
}

bool Entity::addToMap(map::Map* map, EntityUpdater* entityUpdater)
{
	FLAT_ASSERT(map != nullptr && m_map == nullptr);

	updateAABB();

	m_map = map;
	m_cellIndex = map->addEntity(this);
	addedToMap(this, map, entityUpdater);
	positionChanged(m_position);
	headingChanged(m_heading, m_heading);
	elevationChanged(m_elevation);

	updateAABB();

	if (!map->isOnTiles(m_worldSpaceAABB))
	{
		removeFromMap();
		return false;
	}

#ifdef FLAT_DEBUG
	checkSpriteAABB();
#endif
	return true;
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

bool Entity::acceptsPlayerMoveOrder(const flat::Vector2& point, Entity* interactionEntity) const
{
	if (m_movementComponent == nullptr)
	{
		return false;
	}
	else if (m_behaviorComponent == nullptr)
	{
		return true;
	}
	return m_behaviorComponent->handlePlayerMoveOrder(point, interactionEntity);
}

void Entity::moveTo(const flat::Vector2& point, Entity* interactionEntity, bool allowPartialPath)
{
	FLAT_ASSERT(m_movementComponent != nullptr);
	m_movementComponent->moveTo(point, interactionEntity, allowPartialPath);
}

const std::filesystem::path& Entity::getTemplatePath() const
{
	return m_template->getPath();
}

const std::filesystem::path Entity::getTemplateName() const
{
	return m_template->getPath().stem();
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
	else
	{
		if (selected)
		{
			EntityHelper::setSelectedColor(this);
		}
		else
		{
			EntityHelper::clearSelectedColor(this);
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

map::TileIndex Entity::getTileIndexFromPosition() const
{
	FLAT_ASSERT(m_map != nullptr);
	map::TileIndex tileIndex = m_map->getTileIndex(m_position.x, m_position.y);
	FLAT_ASSERT_MSG(map::isValidTile(tileIndex), "Trying to get a tile that does not exist at position (%f, %f)", m_position.x, m_position.y);
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
	m_behaviorComponent  = findComponent<component::behavior::BehaviorComponent>();
	m_collisionComponent = findComponent<component::collision::CollisionComponent>();
	m_movementComponent  = findComponent<component::movement::MovementComponent>();
	m_spriteComponent    = findComponent<component::sprite::SpriteComponent>();
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
		FLAT_ASSERT_MSG(false, "Entity %s should not be busy right after components init", getTemplatePath().string().c_str());
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

void Entity::reset()
{
	deinitComponents();
	initComponents();
	setHeading(0.f);
	setElevation(0.f);
	m_aabbCanChange = true;
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
	if (flat::render::BaseSprite* sprite = getSprite())
	{
		flat::AABB2 spriteAABB;
		sprite->getAABB(spriteAABB);
		FLAT_ASSERT(spriteAABB == getAABB());
	}
}

void Entity::checkValidPosition(const flat::Vector3& potentialPosition) const
{
	const map::TileIndex tileIndex = m_map->getTileIndex(flat::Vector2(potentialPosition));
	FLAT_ASSERT(map::isValidTile(tileIndex));

	if (!hasComponent<component::collision::CollisionComponent>())
	{
		return;
	}

	if (!hasComponent<component::prop::PropComponent>() && !hasComponent<component::projectile::ProjectileComponent>())
	{
		const map::Navigability navigabilityMask = EntityHelper::getNavigabilityMask(this);
		FLAT_ASSERT(m_map->isTileNavigable(tileIndex, navigabilityMask));
	}

	const float tileZ = m_map->getTileZ(tileIndex);
	FLAT_ASSERT(potentialPosition.z >= tileZ - MIN_Z_EPSILON);
}

void Entity::checkValidPosition2d(const flat::Vector2& potentialPosition) const
{
	checkValidPosition(flat::Vector3(potentialPosition.x, potentialPosition.y, m_position.z));
}
#endif

} // entity
} // game


