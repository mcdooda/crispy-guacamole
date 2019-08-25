#include "movementcomponent.h"
#include "movementcomponenttemplate.h"
#include "../collision/collisioncomponent.h"
#include "../projectile/projectilecomponent.h"
#include "../../../entity.h"
#include "../../../entityhelper.h"
#include "../../../entitytemplate.h"
#include "../../../../map/map.h"
#include "../../../../map/tile.h"
#include "../../../../map/scopednavigabilityalteration.h"
#include "../../../../map/pathfinder/pathfinder.h"
#include "../../../../map/pathfinder/zonepathfinder.h"

namespace game
{
namespace entity
{
namespace component
{
namespace movement
{

void MovementComponent::init()
{
	const MovementComponentTemplate* movementComponentTemplate = getTemplate();

	m_nextPathPointIndex = INVALID_POINT_INDEX;
	m_movementSpeed = movementComponentTemplate->getDefaultSpeed();
	m_currentVerticalSpeed = 0.f;
	m_isTouchingGround = movementComponentTemplate->getSnapToGround();

	m_isStrafing = false;
	m_wasMovingLastFrame = false;

	if (m_isTouchingGround)
	{
		// snap the entity to the map's collision when it is actually spawned into the map
		m_owner->addedToMap.on(this, &MovementComponent::snapEntityToTile);
	}

	collision::CollisionComponent* collisionComponent = m_owner->getComponent<collision::CollisionComponent>();
	if (collisionComponent != nullptr)
	{
		collisionComponent->onCollidedWithMap.on(this, &MovementComponent::collidedWithMap);
	}
}

void MovementComponent::deinit()
{
	collision::CollisionComponent* collisionComponent = m_owner->getComponent<collision::CollisionComponent>();
	if (collisionComponent != nullptr)
	{
		collisionComponent->onCollidedWithMap.off(this);
	}
}

void MovementComponent::update(float currentTime, float elapsedTime)
{
#ifdef FLAT_DEBUG
	m_debugSteering = flat::Vector2(0.f, 0.f);
#endif

	if (isMovingAlongPath())
	{
		progressAlongPath(elapsedTime);
	}
	
	if (!m_isTouchingGround)
	{
		fall(elapsedTime);
	}

	triggerStartStopCallbacks();
}

bool MovementComponent::isBusy() const
{
	return isMovingAlongPath() || (!isTouchingGround() && getTemplate()->getSnapToGround());
}

void MovementComponent::cancelCurrentAction()
{
	stopMovement();
}

void MovementComponent::moveTo(const flat::Vector2& destination, Entity* interactionEntity)
{
	// pathfind to destination from the current path's last point or from the entity position

	flat::Vector2 startingPoint;
	if (!isMovingAlongPath())
	{
		const flat::Vector3& position = m_owner->getPosition();
		startingPoint.x = position.x;
		startingPoint.y = position.y;
	}
	else
	{
		startingPoint = m_currentPath.back();
	}

	if (flat::distance2(destination, startingPoint) < flat::square(MIN_DISTANCE_TO_DESTINATION))
	{
		// the destination is close, no need to move
		return;
	}
	
	map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map != nullptr);

	const MovementComponentTemplate* movementComponentTemplate = getTemplate();
	const float jumpHeight = movementComponentTemplate->getJumpMaxHeight();
	map::Navigability navigabilityMask = movementComponentTemplate->getNavigabilityMask();

	// get the right pathfinder class depending on if the movement is restricted to a given zone or not
	map::pathfinder::Pathfinder* pathfinder = nullptr;
	if (const map::Zone* zone = m_restrictionZone.lock().get())
	{
		pathfinder = FLAT_STACK_ALLOCATE(map::pathfinder::ZonePathfinder)(*map, jumpHeight, navigabilityMask, zone);
	}
	else
	{
		pathfinder = FLAT_STACK_ALLOCATE(map::pathfinder::Pathfinder)(*map, jumpHeight, navigabilityMask);
	}

	// ensure that the interaction entity is reachable by temporarily changing its tiles' navigability to match with the current entity
	// it happens when a building entity is blocking all navigability
	map::ScopedNavigabilityAlteration navigabilityAlteration(*map);
	if (interactionEntity != nullptr)
	{
		EntityHelper::eachEntityTile(
			interactionEntity,
			[&map, &navigabilityAlteration, navigabilityMask](map::TileIndex tileIndex)
			{
				navigabilityAlteration.setTileNavigability(tileIndex, navigabilityMask);
			}
		);
	}

	std::vector<flat::Vector2> path;
	if (pathfinder->findPath(startingPoint, destination, path))
	{
		FLAT_ASSERT(path.size() >= 2);
		// insert the new path at the end of the current path
		// the first point of the new path is not inserted as it is the same as the last point of the current path
		m_currentPath.insert(m_currentPath.end(), path.begin() + 1, path.end());
	}
	else
	{
		// if the destination is not reachable, still move towards that direction
		m_currentPath.push_back(destination);
	}

	if (!isMovingAlongPath())
	{
		startMovement();
	}

	FLAT_STACK_DESTROY(pathfinder, Pathfinder);
}

void MovementComponent::jump()
{
	FLAT_ASSERT(m_isTouchingGround);
	m_isTouchingGround = false;
	m_currentVerticalSpeed = getTemplate()->getJumpForce();
}

bool MovementComponent::isMovingAlongPath() const
{
	return m_nextPathPointIndex != INVALID_POINT_INDEX;
}

flat::Vector2 MovementComponent::getCurrentMovementDirection() const
{
	FLAT_ASSERT(isMovingAlongPath());
	return m_currentPath[m_nextPathPointIndex] - m_owner->getPosition2d();
}

const flat::Vector2& MovementComponent::getNextPathPoint() const
{
	FLAT_ASSERT(isMovingAlongPath());
	return m_currentPath[m_nextPathPointIndex];
}

void MovementComponent::progressAlongPath(float elapsedTime)
{
	FLAT_ASSERT(isMovingAlongPath());

	const flat::Vector2 currentMovementDirection = getCurrentMovementDirection();
	flat::Vector2 steering = currentMovementDirection;
	avoidClosestEntity(steering);

#ifdef FLAT_DEBUG
	m_debugSteering = steering;
#endif


	steering = flat::normalize(steering);

	jumpIfNecessary(steering);

	if (!m_isStrafing)
	{
		m_owner->setHeading(flat::vector2_angle(steering), MIN_HEADING_CHANGE);
	}

	const flat::Vector2& position2d = m_owner->getPosition2d();
	flat::Vector2 newPosition2d = position2d + steering * m_movementSpeed * elapsedTime;

	// check if the new position has overtaken the next path point and move it a bit backwards if necessary
	const flat::Vector2& nextPathPoint = getNextPathPoint();
	flat::Vector2 newMovementDirection = nextPathPoint - newPosition2d;
	const bool nextPathPointOvertaken = flat::dot(currentMovementDirection, newMovementDirection) <= 0.f;
	if (nextPathPointOvertaken)
	{
		newPosition2d = nextPathPoint;
		newMovementDirection = nextPathPoint - newPosition2d;
	}
	m_owner->setPosition2d(newPosition2d);

	// has the entity reached the next point on the planned path?
	const bool reachedNextPathPoint = nextPathPointOvertaken || flat::length2(newMovementDirection) < flat::square(EntityHelper::getRadius(m_owner));
	if (reachedNextPathPoint)
	{
		if (m_nextPathPointIndex < m_currentPath.size() - 1)
		{
			++m_nextPathPointIndex;
		}
		else
		{
			stopMovement();
		}
	}
}


void MovementComponent::avoidClosestEntity(flat::Vector2& steering) const
{
	const collision::CollisionComponentTemplate* collisionComponentTemplate = getTemplate<collision::CollisionComponent>();
	if (collisionComponentTemplate == nullptr || !collisionComponentTemplate->shouldSeparateFromOtherEntities())
	{
		// the entity has no collision or does not avoid other entities, nothing to do
		return;
	}

	const flat::Vector2& position2d = m_owner->getPosition2d();

	// entity relative matrix
	flat::Matrix4 transform2d;
	flat::translateBy(transform2d, position2d);
	flat::rotateZBy(transform2d, flat::vector2_angle(steering));
	const flat::Matrix4 transform2dInverse = flat::inverse(transform2d);

	const Entity* entityToAvoid = getClosestEntityToAvoid(steering, transform2dInverse);
	if (entityToAvoid != nullptr)
	{
		const float entityRadius = collisionComponentTemplate->getRadius();

		const float entityToAvoidRadius = EntityHelper::getRadius(entityToAvoid);

		flat::Vector2 relativeEntityPosition2d(transform2dInverse * flat::Vector4(entityToAvoid->getPosition2d(), 0.f, 1.f));
		const float avoidDistance = (entityRadius + entityToAvoidRadius) * flat::SQRT_2;
		const float ySign = flat::sign(relativeEntityPosition2d.y);
		flat::Vector2 relativeAvoidPosition2d(relativeEntityPosition2d.x, relativeEntityPosition2d.y - ySign * avoidDistance);

		flat::Vector2 absoluteAvoidPosition2d(transform2d * flat::Vector4(relativeAvoidPosition2d, 0.f, 1.f));
		steering = absoluteAvoidPosition2d - position2d;
	}
}

const Entity* MovementComponent::getClosestEntityToAvoid(const flat::Vector2& steering, const flat::Matrix4& entityTransform2dInverse) const
{
	const map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map != nullptr);

	const collision::CollisionComponentTemplate* collisionComponentTemplate = m_owner->getComponentTemplate<collision::CollisionComponent>();
	FLAT_ASSERT(collisionComponentTemplate != nullptr);
	FLAT_ASSERT(collisionComponentTemplate->shouldSeparateFromOtherEntities());
	const float entityRadius = collisionComponentTemplate->getRadius();

	const flat::Vector2& entityPosition2d = m_owner->getPosition2d();

	flat::Vector2 avoidanceCenter;
	float avoidanceRadius;
	getAvoidanceArea(avoidanceCenter, avoidanceRadius);

	const Entity* entityToAvoid = nullptr;
	float entityToAvoidDistance = std::numeric_limits<float>::max();

	map->eachEntityInRange(
		avoidanceCenter,
		avoidanceRadius,
		[this, &steering, entityPosition2d, entityTransform2dInverse, entityRadius, &entityToAvoid, &entityToAvoidDistance](Entity* otherEntity)
		{
			// if the entities cannot collide, nothing to do
			if (EntityHelper::canCollide(m_owner, otherEntity))
			{
				return;
			}

			// do not avoid projectiles
			if (otherEntity->hasComponent<projectile::ProjectileComponent>())
			{
				return;
			}

			// check if the other entity is static or moving in the same direction
			const movement::MovementComponent* otherMovementComponent = otherEntity->getComponent<movement::MovementComponent>();
			if (otherMovementComponent == nullptr
				|| (otherMovementComponent->isMovingAlongPath() && flat::dot(steering, otherMovementComponent->getCurrentMovementDirection()) > 0.f))
			{
				return;
			}

			const flat::Vector2& otherPosition2d = otherEntity->getPosition2d();
			const float otherEntityRadius = EntityHelper::getRadius(otherEntity);

			const float avoidDistance = (entityRadius + otherEntityRadius) * flat::SQRT_2;

			const flat::Vector2 relativeOtherPosition2d(entityTransform2dInverse * flat::Vector4(otherPosition2d, 0.f, 1.f));
			if (std::abs(relativeOtherPosition2d.y) < avoidDistance && relativeOtherPosition2d.x < entityToAvoidDistance)
			{
				entityToAvoidDistance = relativeOtherPosition2d.x;
				entityToAvoid = otherEntity;
			}
		}
	);

	return entityToAvoid;
}

void MovementComponent::getAvoidanceArea(flat::Vector2& center, float& radius) const
{
	FLAT_ASSERT(isMovingAlongPath());
	const flat::Vector2& nextPathPoint = getNextPathPoint();
	const flat::Vector2 position2d(m_owner->getPosition());
	const float nextPathPointDistance = flat::distance(position2d, nextPathPoint);
	if (nextPathPointDistance > MAX_AVOIDANCE_RADIUS * 2.f)
	{
		const flat::Vector2 movementDirection = flat::normalize(getCurrentMovementDirection());
		center = position2d + movementDirection * MAX_AVOIDANCE_RADIUS;
		radius = MAX_AVOIDANCE_RADIUS;
	}
	else
	{
		center = (position2d + nextPathPoint) * 0.5f;
		radius = nextPathPointDistance * 0.5f;
	}
}

void MovementComponent::fall(float elapsedTime)
{
	FLAT_ASSERT(!m_isTouchingGround);

	const float gravityForce = -getTemplate()->getWeight();

	const float previousVerticalSpeed = m_currentVerticalSpeed;
	m_currentVerticalSpeed += gravityForce * elapsedTime;

	flat::Vector3 newPosition = m_owner->getPosition();
	newPosition.z += (previousVerticalSpeed + m_currentVerticalSpeed) * 0.5f * elapsedTime;
	m_owner->setPosition(newPosition);
}

void MovementComponent::startMovement()
{
	FLAT_ASSERT(!isMovingAlongPath());
	FLAT_ASSERT(!m_currentPath.empty());
	m_nextPathPointIndex = 0;
}

void MovementComponent::stopMovement()
{
	if (isMovingAlongPath())
	{
		m_currentPath.clear();
		m_nextPathPointIndex = INVALID_POINT_INDEX;
	}
}

void MovementComponent::triggerStartStopCallbacks()
{
	const bool isMovingThisFrame = isMovingAlongPath();
	if (m_wasMovingLastFrame && !isMovingThisFrame)
	{
		movementStopped();
	}
	else if (!m_wasMovingLastFrame && isMovingThisFrame)
	{
		movementStarted();
	}
	m_wasMovingLastFrame = isMovingThisFrame;
}

void MovementComponent::jumpIfNecessary(const flat::Vector2& steering)
{
	if (!m_isTouchingGround)
	{
		return;
	}

	const map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map != nullptr);

	// very raw approximation of what the next tile could be, could take the whole path into account for more precision
	flat::Vector2 nextTilePosition = m_owner->getPosition2d() + steering * 0.4f;
	const map::Navigability navigabilityMask = getTemplate()->getNavigabilityMask();
	const map::TileIndex nextTileIndex = map->getTileIndexIfNavigable(nextTilePosition.x, nextTilePosition.y, navigabilityMask);

	if (map::isValidTile(nextTileIndex))
	{
		const float nextTileZ = map->getTileZ(nextTileIndex);
		if (!flat::areValuesClose(m_owner->getPosition().z, nextTileZ, JUMP_MIN_Z_DIFFERENCE))
		{
			jump();
		}
	}
}

bool MovementComponent::snapEntityToTile(Entity* entity, map::Map* map)
{
	FLAT_ASSERT(entity == m_owner);
	FLAT_ASSERT(m_isTouchingGround);

	const map::TileIndex tileIndex = m_owner->getTileIndexFromPosition();
	const float tileZ = map->getTileZ(tileIndex);
	m_owner->setZ(tileZ);

	return false;
}

bool MovementComponent::collidedWithMap(map::TileIndex tileIndex, const flat::Vector3& normal)
{
	if (!m_isTouchingGround)
	{
		if (normal.z > flat::SQRT_2_DIV_2) // the collision normal is vertical and upwards
		{
			m_owner->setZ(m_owner->getMap()->getTileZ(tileIndex));
			m_isTouchingGround = true;
			m_currentVerticalSpeed = 0.f;
		}
	}
	return true;
}

#ifdef FLAT_DEBUG
void MovementComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	debugDrawCurrentPath(debugDisplay);
	debugDrawSteering(debugDisplay);
	debugDrawAvoidanceArea(debugDisplay);
	debugDrawEntity(debugDisplay);
}

void MovementComponent::debugDrawCurrentPath(debug::DebugDisplay& debugDisplay) const
{
	if (!isMovingAlongPath())
	{
		return;
	}

	const map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map != nullptr);

	flat::Vector3 previousPoint = m_owner->getPosition();
	for (int i = m_nextPathPointIndex, e = static_cast<int>(m_currentPath.size()); i < e; ++i)
	{
		const flat::Vector2& point2d = m_currentPath[i];
		const map::TileIndex tileIndex = map->getTileIndex(point2d);
		FLAT_ASSERT(map::isValidTile(tileIndex));
		flat::Vector3 point(point2d, map->getTileZ(tileIndex));
		debugDisplay.add3dLine(previousPoint, point);
		previousPoint = point;
	}
}

void MovementComponent::debugDrawSteering(debug::DebugDisplay& debugDisplay) const
{
	const flat::Vector3 steering3d(m_debugSteering, 0.f);
	debugDisplay.add3dLine(m_owner->getPosition(), m_owner->getPosition() + steering3d, flat::video::Color::RED);
}

void MovementComponent::debugDrawAvoidanceArea(debug::DebugDisplay& debugDisplay) const
{
	if (!isMovingAlongPath())
	{
		return;
	}

	flat::Vector2 avoidanceCenter;
	float avoidanceRadius;
	getAvoidanceArea(avoidanceCenter, avoidanceRadius);
	flat::Vector3 avoidanceCenter3d(avoidanceCenter, m_owner->getPosition().z);
	debugDisplay.add3dCircle(avoidanceCenter3d, avoidanceRadius, flat::video::Color::BLUE);
}

void MovementComponent::debugDrawEntity(debug::DebugDisplay& debugDisplay) const
{
	const map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map != nullptr);

	flat::video::Color color = m_isTouchingGround ? flat::video::Color::BLUE : flat::video::Color::RED;
	const flat::Vector3& position = m_owner->getPosition();
	debugDisplay.add3dCircle(position, EntityHelper::getRadius(m_owner), color);

	if (!m_isTouchingGround)
	{
		const flat::Vector2 position2d(position);
		const map::TileIndex tileIndex = map->getTileIndex(position2d);
		FLAT_ASSERT(map::isValidTile(tileIndex));
		flat::Vector3 positionOnTile(position2d, map->getTileZ(tileIndex));
		debugDisplay.add3dLine(position, positionOnTile, flat::video::Color::RED);
	}
}
#endif

} // movement
} // component
} // entity
} // game
