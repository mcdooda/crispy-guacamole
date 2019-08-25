#include "movementcomponent.h"
#include "movementcomponenttemplate.h"
#include "../collision/collisioncomponent.h"
#include "../projectile/projectilecomponent.h"
#include "../../../entity.h"
#include "../../../entityhelper.h"
#include "../../../entitytemplate.h"
#include "../../../../map/map.h"
#include "../../../../map/tile.h"
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
	FLAT_DEBUG_ONLY(m_steering = flat::Vector2(0.f, 0.f);)

	if (isMovingAlongPath())
	{
		progressAlongPath(elapsedTime);
	}
	
	if (!m_isTouchingGround)
	{
		fall(elapsedTime);
	}

	triggerStartStopCallbacks();

	m_wasMovingLastFrame = isMovingAlongPath();
}

bool MovementComponent::isBusy() const
{
	return isMovingAlongPath() || (!isTouchingGround() && getTemplate()->getSnapToGround());
}

void MovementComponent::cancelCurrentAction()
{
	stopMovement();
}

bool MovementComponent::isMovingAlongPath() const
{
	return m_nextPathPointIndex != INVALID_POINT_INDEX;
}

flat::Vector2 MovementComponent::getCurrentMovementDirection() const
{
	FLAT_ASSERT(isMovingAlongPath());
	return m_currentPath[m_nextPathPointIndex] - flat::Vector2(m_owner->getPosition());
}

void MovementComponent::moveTo(const flat::Vector2& point, Entity* interactionEntity)
{
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
	
	if (flat::distance2(point, startingPoint) > flat::square(MIN_DISTANCE_TO_DESTINATION))
	{
		map::Map* map = m_owner->getMap();
		FLAT_ASSERT(map != nullptr);

		const MovementComponentTemplate* movementComponentTemplate = getTemplate();
		const float jumpHeight = movementComponentTemplate->getJumpMaxHeight();
		map::Navigability navigabilityMask = movementComponentTemplate->getNavigabilityMask();

		map::pathfinder::Pathfinder* pathfinder = nullptr;
		if (const map::Zone* zone = m_restrictionZone.lock().get())
		{
			pathfinder = static_cast<map::pathfinder::ZonePathfinder*>(alloca(sizeof(map::pathfinder::ZonePathfinder)));
			new (pathfinder) map::pathfinder::ZonePathfinder(*map, jumpHeight, navigabilityMask, zone);
		}
		else
		{
			pathfinder = static_cast<map::pathfinder::Pathfinder*>(alloca(sizeof(map::pathfinder::Pathfinder)));
			new (pathfinder) map::pathfinder::Pathfinder(*map, jumpHeight, navigabilityMask);
		}

		map::Navigability initialTileNavigability = map::Navigability::NONE;

		flat::containers::HybridArray<map::TileIndex, 4> interactionTileIndices;
		flat::containers::HybridArray<map::Navigability, 4> interactionTileInitialNavigabilities;

		if (interactionEntity != nullptr)
		{
			EntityHelper::eachEntityTile(
				interactionEntity,
				[&map, &interactionTileIndices, &interactionTileInitialNavigabilities, navigabilityMask](map::TileIndex tileIndex)
				{
					if (!map->isTileNavigable(tileIndex, navigabilityMask))
					{
						interactionTileIndices.add(tileIndex);
						interactionTileInitialNavigabilities.add(map->getTileNavigability(tileIndex));
						map->setTileNavigability(tileIndex, navigabilityMask);
					}
				}
			);
		}

		std::vector<flat::Vector2> path;
		if (pathfinder->findPath(startingPoint, point, path))
		{
			FLAT_ASSERT(path.size() >= 2);
			// insert the new path at the end of the current path
			// the first point of the new path is not inserted as it is the same as the last point of the current path
			m_currentPath.insert(m_currentPath.end(), path.begin() + 1, path.end());
		}
		else
		{
			// if the destination is not reachable, still move towards that direction
			m_currentPath.push_back(point);
		}

		if (!isMovingAlongPath())
		{
			startMovement();
		}

		pathfinder->~Pathfinder();

		FLAT_ASSERT(interactionTileIndices.getSize() == interactionTileInitialNavigabilities.getSize());
		for (unsigned int i = 0, e = interactionTileIndices.getSize(); i < e; ++i)
		{
			map->setTileNavigability(interactionTileIndices[i], interactionTileInitialNavigabilities[i]);
		}
	}
}

void MovementComponent::jump()
{
	FLAT_ASSERT(m_isTouchingGround);
	m_isTouchingGround = false;
	m_currentVerticalSpeed = getTemplate()->getJumpForce();
}

void MovementComponent::progressAlongPath(float elapsedTime)
{
	const flat::Vector2& nextPathPoint = getNextPathPoint();
	const flat::Vector3& position = m_owner->getPosition();
	flat::Vector2 position2d = flat::Vector2(position.x, position.y);
	const flat::Vector2 move = nextPathPoint - position2d;
	const float moveLen2 = flat::length2(move);
	if (moveLen2 > 0.f)
	{
		const map::Map* map = m_owner->getMap();

		// entity relative matrix
		flat::Matrix4 transform2d;
		flat::translateBy(transform2d, position2d);
		flat::rotateZBy(transform2d, flat::vector2_angle(move));
		flat::Matrix4 transform2dInverse = flat::inverse(transform2d);

		flat::Vector2 steering = move;

		// entity radius
		const collision::CollisionComponentTemplate* collisionComponentTemplate = getTemplate<collision::CollisionComponent>();
		float radius = 0.f;
		if (collisionComponentTemplate != nullptr && collisionComponentTemplate->getSeparate())
		{
			radius = collisionComponentTemplate->getRadius();

			Entity* entityToAvoid = nullptr;
			float entityToAvoidDistance = std::numeric_limits<float>::max();

			flat::Vector2 avoidanceCenter;
			float avoidanceRadius;
			getAvoidanceArea(avoidanceCenter, avoidanceRadius);

			// find entities moving in the opposite direction and avoid them
			map->eachEntityInRange(
				avoidanceCenter,
				avoidanceRadius,
				[this, &position2d, &move, moveLen2, &transform2dInverse, radius, &entityToAvoid, &entityToAvoidDistance, collisionComponentTemplate](Entity* entity)
				{
					if (entity == m_owner)
						return;

					if (entity->getComponent<projectile::ProjectileComponent>() != nullptr)
						return;

					// in front of the component owner?
					flat::Vector2 entityPosition2d(entity->getPosition());
					if (flat::dot(move, entityPosition2d - position2d) > 0.f
						&& flat::length2(entityPosition2d - position2d) < moveLen2)
					{
						movement::MovementComponent* entityMovementComponent = entity->getComponent<movement::MovementComponent>();
						// moving in opposite direction?
						// TODO: also avoid if the other entity moves slower than the current entity
						if (!entityMovementComponent
							|| !entityMovementComponent->isMovingAlongPath()
							|| flat::dot(getCurrentMovementDirection(), entityMovementComponent->getCurrentMovementDirection()) < 0.f)
						{
							const std::shared_ptr<const EntityTemplate>& entityTemplate = entity->getEntityTemplate();
							const collision::CollisionComponentTemplate* otherCollisionComponentTemplate = entityTemplate->getComponentTemplate<collision::CollisionComponent>();
							if (otherCollisionComponentTemplate != nullptr && otherCollisionComponentTemplate->getSeparate()
								&& !(otherCollisionComponentTemplate == collisionComponentTemplate && !collisionComponentTemplate->getSeparateSameType()))
							{
								const float entityRadius = collisionComponentTemplate->getRadius();
								const float avoidDistance = (radius + entityRadius) * flat::SQRT_2;

								// compute the other entity's position relatively to the current entity's position and heading
								flat::Vector2 relativeEntityPosition2d = flat::Vector2(transform2dInverse * flat::Vector4(entityPosition2d, 0.f, 1.f));
								//FLAT_ASSERT(relativeEntityPosition2d.x > 0.f);
								if (std::abs(relativeEntityPosition2d.y) <= avoidDistance)
								{
									if (relativeEntityPosition2d.x < entityToAvoidDistance)
									{
										entityToAvoidDistance = relativeEntityPosition2d.x;
										entityToAvoid = entity;
									}
								}
							}
						}
					}
				}
			);

			if (entityToAvoid != nullptr)
			{
				const float entityToAvoidRadius = EntityHelper::getRadius(entityToAvoid);

				flat::Vector2 relativeEntityPosition2d(transform2dInverse * flat::Vector4(entityToAvoid->getPosition(), 1.f));
				const float avoidDistance = (radius + entityToAvoidRadius) * flat::SQRT_2;
				const float ySign = flat::sign(relativeEntityPosition2d.y);
				flat::Vector2 relativeAvoidPosition2d(relativeEntityPosition2d.x, relativeEntityPosition2d.y - ySign * avoidDistance);

				flat::Vector2 absoluteAvoidPosition2d(transform2d * flat::Vector4(relativeAvoidPosition2d, 0.f, 1.f));
				steering = absoluteAvoidPosition2d - position2d;
			}
		}

		FLAT_DEBUG_ONLY(m_steering = steering;)
			steering = flat::normalize(steering);
		if (!m_isStrafing)
		{
			m_owner->setHeading(flat::vector2_angle(steering), flat::PI_2 / 64.f);
		}
		flat::Vector2 newPosition2d = position2d + steering * m_movementSpeed * elapsedTime;

		flat::Vector2 nextTilePosition = position2d + steering * 0.4f;
		const map::Navigability navigabilityMask = getTemplate()->getNavigabilityMask();
		const map::TileIndex nextTileIndex = map->getTileIndexIfNavigable(nextTilePosition.x, nextTilePosition.y, navigabilityMask);

		// jump if necessary
		if (nextTileIndex != map::TileIndex::INVALID_TILE)
		{
			const float nextTileZ = map->getTileZ(nextTileIndex);
			if (m_isTouchingGround
				&& (nextTileZ > position.z + MIN_Z_EPSILON || nextTileZ < position.z - MIN_Z_EPSILON))
			{
				jump();
			}
		}

		// has the entity reached the next point on the planned path?
		flat::Vector2 newMove = nextPathPoint - newPosition2d;
		const bool pointOvertook = flat::dot(move, newMove) <= 0.f;
		const bool lastPoint = m_nextPathPointIndex == m_currentPath.size() - 1;
		if (pointOvertook || (!lastPoint && flat::length2(newMove) < flat::square(radius)))
		{
			if (pointOvertook)
			{
				newPosition2d = nextPathPoint;
			}
			if (lastPoint)
			{
				stopMovement();
			}
			else
			{
				++m_nextPathPointIndex;
			}
		}
		m_owner->setXY(newPosition2d);
	}
}

const flat::Vector2& MovementComponent::getNextPathPoint() const
{
	FLAT_ASSERT(isMovingAlongPath());
	return m_currentPath[m_nextPathPointIndex];
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
		FLAT_ASSERT(tileIndex != map::TileIndex::INVALID_TILE);
		flat::Vector3 point(point2d, map->getTileZ(tileIndex));
		debugDisplay.add3dLine(previousPoint, point);
		previousPoint = point;
	}
}

void MovementComponent::debugDrawSteering(debug::DebugDisplay& debugDisplay) const
{
	const flat::Vector3 steering3d(m_steering, 0.f);
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
		FLAT_ASSERT(tileIndex != map::TileIndex::INVALID_TILE);
		flat::Vector3 positionOnTile(position2d, map->getTileZ(tileIndex));
		debugDisplay.add3dLine(position, positionOnTile, flat::video::Color::RED);
	}
}
#endif

} // movement
} // component
} // entity
} // game



