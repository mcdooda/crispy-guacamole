#include "movementcomponent.h"
#include "movementcomponenttemplate.h"
#include "../collision/collisioncomponent.h"
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
	m_owner->setCanBeSelected(true);

	const MovementComponentTemplate* movementComponentTemplate = getTemplate();
	m_speed = movementComponentTemplate->getSpeed();
	// not yet in the map but makes the component not busy
	m_isTouchingGround = movementComponentTemplate->getSnapToGround();

	m_owner->addedToMap.on(this, &MovementComponent::addedToMap);
}

void MovementComponent::deinit()
{
	m_owner->addedToMap.off(this);
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
			addPointOnPath(m_destination);
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
				map->eachEntityInRange(position2d, 3.f, [this, &position2d, &move, moveLen2, &transform2dInverse, radius, &entityToAvoid, &entityToAvoidDistance](Entity* entity)
				{
					if (entity == m_owner)
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
							const collision::CollisionComponentTemplate* collisionComponentTemplate = entityTemplate->getComponentTemplate<collision::CollisionComponent>();
							if (collisionComponentTemplate != nullptr && collisionComponentTemplate->getSeparate())
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
				});

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
			m_owner->setHeading(flat::vector2_angle(steering));
			flat::Vector2 newPosition2d = position2d + steering * m_speed * elapsedTime;
			
			flat::Vector2 nextTilePosition = position2d + steering * 0.4f;
			const map::Tile* nextTile = map->getTileIfWalkable(nextTilePosition.x, nextTilePosition.y);
			
			// jump if necessary
			if (nextTile && (nextTile->getZ() > position.z + MIN_Z_EPSILON || nextTile->getZ() < position.z - MIN_Z_EPSILON))
			{
				jump();
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

	const bool wasMoving = m_isMoving;
	m_isMoving = followsPath();
	if (!wasMoving && m_isMoving)
	{
		movementStarted();
	}
	else if (!m_isMoving && wasMoving)
	{
		m_returnToDestinationTime = currentTime + RETURN_TO_DESTINATION_DURATION;
		movementStopped();
	}
}

bool MovementComponent::addedToMap(Entity* entity, map::Map* map)
{
	FLAT_ASSERT(entity == m_owner);

	const map::Tile* tile = m_owner->getTile();
	const flat::Vector3& position = m_owner->getPosition();

	m_destination = flat::Vector2(position);

	// m_isTouchingGround already set is init()
	FLAT_ASSERT(m_isTouchingGround == getTemplate()->getSnapToGround());
	if (m_isTouchingGround || tile->getZ() >= position.z)
	{
		m_owner->setZ(tile->getZ());
		m_isTouchingGround = true; // in case the entity was below its tile
	}
	m_zSpeed = 0.f;
	return true;
}

bool MovementComponent::isBusy() const
{
	return followsPath() || (!isTouchingGround() && getTemplate()->getSnapToGround());
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

void MovementComponent::addPointOnPath(const flat::Vector2& point)
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
	
	if (flat::length2(point - startingPoint) > MIN_DISTANCE_TO_DESTINATION * MIN_DISTANCE_TO_DESTINATION)
	{
		const map::Map& map = *m_owner->getMap();
		const float jumpHeight = getTemplate()->getJumpMaxHeight();

		map::pathfinder::Pathfinder* pathfinder = nullptr;
		if (const map::Zone* zone = m_restrictToZone.lock().get())
		{
			pathfinder = static_cast<map::pathfinder::ZonePathfinder*>(alloca(sizeof(map::pathfinder::ZonePathfinder)));
			new (pathfinder) map::pathfinder::ZonePathfinder(map, jumpHeight, zone);
		}
		else
		{
			pathfinder = static_cast<map::pathfinder::Pathfinder*>(alloca(sizeof(map::pathfinder::Pathfinder)));
			new (pathfinder) map::pathfinder::Pathfinder(map, jumpHeight);
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
	}
}

void MovementComponent::clearPath()
{
	m_path.clear();
}

void MovementComponent::jump()
{
	if (m_isTouchingGround)
	{
		m_zSpeed = getTemplate()->getJumpForce();
		m_isTouchingGround = false;
	}
}

void MovementComponent::fall(float elapsedTime)
{
	if (m_isTouchingGround)
		return;
		
	const map::Tile* tile = m_owner->getTile();
	const float acceleration = getTemplate()->getWeight();
	const float oldZSpeed = m_zSpeed;
	m_zSpeed -= acceleration * elapsedTime;
	const flat::Vector3& position = m_owner->getPosition();
	float z = position.z + (oldZSpeed + m_zSpeed) * 0.5f * elapsedTime;
	if (z < tile->getZ())
	{
		z = tile->getZ();
		m_isTouchingGround = true;
	}
	m_owner->setZ(z);
}

#ifdef FLAT_DEBUG
void MovementComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	const map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map != nullptr);
	flat::Vector3 previousPoint = m_owner->getPosition();
	for (flat::Vector2 point2d : m_path)
	{
		const map::Tile* tile = map->getTileIfExists(point2d.x, point2d.y);
		FLAT_ASSERT(tile != nullptr);
		flat::Vector3 point(point2d, tile->getZ());
		debugDisplay.add3dLine(previousPoint, point);
		previousPoint = point;
	}

	debugDisplay.add3dLine(m_owner->getPosition(), m_owner->getPosition() + flat::Vector3(m_steering, 0.f), flat::video::Color::RED);
}
#endif

} // movement
} // component
} // entity
} // game



