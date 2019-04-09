#include "movementcomponent.h"
#include "movementcomponenttemplate.h"
#include "../collision/collisioncomponent.h"
#include "../projectile/projectilecomponent.h"
#include "../../componenttype.h"
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
	m_movementSpeed = movementComponentTemplate->getSpeed();
	// not yet in the map but makes the component not busy
	m_isTouchingGround = movementComponentTemplate->getSnapToGround();

	m_owner->addedToMap.on(this, &MovementComponent::addedToMap);
	m_owner->removedFromMap.on(this, &MovementComponent::removedFromMap);

	collision::CollisionComponent* collisionComponent = m_owner->getComponent<collision::CollisionComponent>();
	if (collisionComponent != nullptr)
	{
		collisionComponent->onCollidedWithMap.on(this, &MovementComponent::collidedWithMap);
	}

	m_isFollowingPath = false;
	m_isStrafing = false;
}

void MovementComponent::deinit()
{
	m_owner->addedToMap.off(this);
	m_owner->removedFromMap.off(this);

	collision::CollisionComponent* collisionComponent = m_owner->getComponent<collision::CollisionComponent>();
	if (collisionComponent != nullptr)
	{
		collisionComponent->onCollidedWithMap.off(this);
	}
}

void MovementComponent::update(float currentTime, float elapsedTime)
{
	FLAT_DEBUG_ONLY(m_steering = flat::Vector2(0.f, 0.f);)

	if (!followsPath())
	{
		flat::Vector2 ownerPosition(m_owner->getPosition());
		if (flat::length2(ownerPosition - m_destination) > MIN_DISTANCE_TO_DESTINATION * MIN_DISTANCE_TO_DESTINATION
			&& currentTime > m_returnToDestinationTime)
		{
			moveTo(m_destination);
		}
	}

	if (followsPath())
	{
		flat::Vector2& pathNextPoint = m_path.front();
		const flat::Vector3& position = m_owner->getPosition();
		flat::Vector2 position2d = flat::Vector2(position.x, position.y);
		const flat::Vector2 move = pathNextPoint - position2d;
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

				// find entities moving in the opposite direction
				map->eachEntityInRange(
					position2d,
					3.f,
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
								|| !entityMovementComponent->followsPath()
								|| flat::dot(getCurrentDirection(), entityMovementComponent->getCurrentDirection()) < 0.f)
							{
								const std::shared_ptr<const EntityTemplate>& entityTemplate = entity->getEntityTemplate();
								const collision::CollisionComponentTemplate* otherCollisionComponentTemplate = entityTemplate->getComponentTemplate<collision::CollisionComponent>();
								if (otherCollisionComponentTemplate != nullptr && otherCollisionComponentTemplate->getSeparate()
									&& !(otherCollisionComponentTemplate == collisionComponentTemplate && !collisionComponentTemplate->getSeparateSameType()))
								{
									const float entityRadius = collisionComponentTemplate->getRadius();
									const float avoidDistance = (radius + entityRadius) * flat::SQRT2;

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
					const float avoidDistance = (radius + entityToAvoidRadius) * flat::SQRT2;
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
				m_owner->setHeading(flat::vector2_angle(steering), flat::PI2 / 64.f);
			}
			flat::Vector2 newPosition2d = position2d + steering * m_movementSpeed * elapsedTime;
			
			flat::Vector2 nextTilePosition = position2d + steering * 0.4f;
			const map::Navigability navigabilityMask = getTemplate()->getNavigabilityMask();
			const map::TileIndex nextTileIndex = map->getTileIndexIfNavigable(nextTilePosition.x, nextTilePosition.y, navigabilityMask);
			
			// jump if necessary
			if (nextTileIndex != map::TileIndex::INVALID_TILE)
			{
				const float nextTileZ = map->getTileZ(nextTileIndex);
				if (nextTileZ > position.z + MIN_Z_EPSILON || nextTileZ < position.z - MIN_Z_EPSILON)
				{
					jump();
				}
			}
			
			// has the entity reached the next point on the planned path?
			flat::Vector2 newMove = pathNextPoint - newPosition2d;
			const bool pointOvertook = flat::dot(move, newMove) <= 0.f;
			const bool lastPoint = m_path.size() == 1;
			if (pointOvertook || (!lastPoint && flat::length2(newMove) < radius * radius))
			{
				if (pointOvertook)
				{
					newPosition2d = pathNextPoint;
				}
				m_path.pop_front();
			}
			m_owner->setXY(newPosition2d);
		}
	}
	
	fall(elapsedTime);

	m_isFollowingPath = followsPath();
}

bool MovementComponent::isBusy() const
{
	return followsPath() || (!isTouchingGround() && getTemplate()->getSnapToGround());
}

void MovementComponent::cancelCurrentAction()
{
	clearPath();
}

bool MovementComponent::followsPath() const
{
	return !m_path.empty();
}

flat::Vector2 MovementComponent::getCurrentDirection() const
{
	FLAT_ASSERT(followsPath());
	return m_path.front() - flat::Vector2(m_owner->getPosition());
}

void MovementComponent::moveTo(const flat::Vector2& point, Entity* interactionEntity)
{
	m_destination = point;

	flat::Vector2 startingPoint;
	if (m_path.empty())
	{
		const flat::Vector3& position = m_owner->getPosition();
		startingPoint.x = position.x;
		startingPoint.y = position.y;
	}
	else
	{
		startingPoint = m_path.back();
	}
	
	if (flat::length2(point - startingPoint) > flat::square(MIN_DISTANCE_TO_DESTINATION))
	{
		map::Map& map = *m_owner->getMap();

		const MovementComponentTemplate* movementComponentTemplate = getTemplate();
		const float jumpHeight = movementComponentTemplate->getJumpMaxHeight();
		map::Navigability navigabilityMask = movementComponentTemplate->getNavigabilityMask();

		map::pathfinder::Pathfinder* pathfinder = nullptr;
		if (const map::Zone* zone = m_restrictToZone.lock().get())
		{
			pathfinder = static_cast<map::pathfinder::ZonePathfinder*>(alloca(sizeof(map::pathfinder::ZonePathfinder)));
			new (pathfinder) map::pathfinder::ZonePathfinder(map, jumpHeight, navigabilityMask, zone);
		}
		else
		{
			pathfinder = static_cast<map::pathfinder::Pathfinder*>(alloca(sizeof(map::pathfinder::Pathfinder)));
			new (pathfinder) map::pathfinder::Pathfinder(map, jumpHeight, navigabilityMask);
		}

		map::Navigability initialTileNavigability;

		map::TileIndex interactionTileIndex = map::TileIndex::INVALID_TILE;
		if (interactionEntity != nullptr)
		{
			map::TileIndex tileIndex = interactionEntity->getTileIndexFromPosition();
			if (!map.isTileNavigable(tileIndex, navigabilityMask))
			{
				initialTileNavigability = map.getTileNavigability(tileIndex);
				map.setTileNavigability(tileIndex, navigabilityMask); // it's cheating because we are setting a mask instead of single navigability
				interactionTileIndex = tileIndex;
			}
		}

		std::vector<flat::Vector2> path;
		if (pathfinder->findPath(startingPoint, point, path))
		{
			path.erase(path.begin());
			for (const flat::Vector2& point : path)
			{
				m_path.push_back(point);
			}
		}
		else
		{
			m_path.push_back(point);
		}

		pathfinder->~Pathfinder();

		if (interactionTileIndex != map::TileIndex::INVALID_TILE)
		{
			map.setTileNavigability(interactionTileIndex, initialTileNavigability);
		}
	}
}

void MovementComponent::clearPath()
{
	m_destination = flat::Vector2(m_owner->getPosition());
	m_returnToDestinationTime = FLT_MAX;
	m_path.clear();
}

void MovementComponent::jump()
{
	if (m_isTouchingGround)
	{
		m_midairVelocity = flat::Vector3(0.f, 0.f, getTemplate()->getJumpForce());
		m_midairAcceleration = flat::Vector3(0.f, 0.f, 0.f);
		m_isTouchingGround = false;
	}
}

void MovementComponent::fall(float elapsedTime)
{
	if (m_isTouchingGround && flat::length2(m_midairAcceleration) < FLT_EPSILON)
		return;

	const flat::Vector3 gravityForce = flat::Vector3(0.f, 0.f, -getTemplate()->getWeight());
	const flat::Vector3 oldVelocity = m_midairVelocity;

	m_midairVelocity += (gravityForce + m_midairAcceleration) * elapsedTime;

	// reset the additional acceleration, has to be set every frame if needed
	m_midairAcceleration = flat::Vector3(0.f, 0.f, 0.f);

	const flat::Vector3& position = m_owner->getPosition();
	flat::Vector3 newPosition = position + (oldVelocity + m_midairVelocity) * 0.5f * elapsedTime;
	m_owner->setPosition(newPosition);
}

void MovementComponent::land()
{
	m_isTouchingGround = true;
	m_midairVelocity = flat::Vector3(0.f, 0.f, 0.f);
	m_midairAcceleration = flat::Vector3(0.f, 0.f, 0.f);
}

bool MovementComponent::addedToMap(Entity* entity, map::Map* map)
{
	FLAT_ASSERT(entity == m_owner);

	const map::TileIndex tileIndex = m_owner->getTileIndexFromPosition();
	const float tileZ = m_owner->getMap()->getTileZ(tileIndex);
	const flat::Vector3& position = m_owner->getPosition();

	m_destination = flat::Vector2(position);

	// m_isTouchingGround already set is init()
	FLAT_ASSERT(m_isTouchingGround == getTemplate()->getSnapToGround());
	if (m_isTouchingGround)
	{
		m_owner->setZ(tileZ);
		m_isTouchingGround = true; // in case the entity was below its tile
	}
	m_midairVelocity = flat::Vector3(0.f, 0.f, 0.f);
	m_midairAcceleration = flat::Vector3(0.f, 0.f, 0.f);

	// connect to position and heading changed slots
	if (m_owner->hasSprite())
	{
		m_owner->positionChanged.on(this, &MovementComponent::updateSpritePosition);
	}

	return true;
}

bool MovementComponent::removedFromMap(Entity* entity)
{
	FLAT_ASSERT(entity == m_owner);

	if (m_owner->hasSprite())
	{
		m_owner->positionChanged.off(this);
	}

	return true;
}

bool MovementComponent::collidedWithMap(map::TileIndex tileIndex, const flat::Vector3& normal)
{
	if (!m_isTouchingGround)
	{
		if (normal.z > 0.7f && m_midairVelocity.z < 0.f)
		{
			m_owner->setZ(m_owner->getMap()->getTileZ(tileIndex));
			land();
		}
	}
	return true;
}

bool MovementComponent::updateSpritePosition(const flat::Vector3& position)
{
	FLAT_ASSERT(m_owner->hasSprite());

	const map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map != nullptr);

	flat::Vector2 position2d(map->getTransform() * position);
	m_owner->getSprite().setPosition(position2d);
	return true;
}

#ifdef FLAT_DEBUG
void MovementComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	const map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map != nullptr);
	flat::Vector3 previousPoint = m_owner->getPosition();
	for (flat::Vector2 point2d : m_path)
	{
		const map::TileIndex tileIndex = map->getTileIndex(point2d.x, point2d.y);
		FLAT_ASSERT(tileIndex != map::TileIndex::INVALID_TILE);
		flat::Vector3 point(point2d, map->getTileZ(tileIndex));
		debugDisplay.add3dLine(previousPoint, point);
		previousPoint = point;
	}

	debugDisplay.add3dLine(m_owner->getPosition(), m_owner->getPosition() + flat::Vector3(m_steering, 0.f), flat::video::Color::RED);

	if (m_isTouchingGround)
	{
		debugDisplay.add3dCircle(m_owner->getPosition(), 0.25f, flat::video::Color::BLUE);
	}
	else
	{
		debugDisplay.add3dCircle(m_owner->getPosition(), 0.4f, flat::video::Color::RED);
	}
}
#endif

} // movement
} // component
} // entity
} // game



