#include "movementcomponent.h"
#include "movementcomponenttemplate.h"
#include "../../componenttype.h"
#include "../../../entity.h"
#include "../../../entitytemplate.h"
#include "../../../../map/map.h"
#include "../../../../map/tile.h"
#include "../../../../map/pathfinder.h"

namespace game
{
namespace entity
{
namespace component
{

void MovementComponent::init()
{
	m_owner->addedToMap.on(this, &MovementComponent::addedToMap);
}

void MovementComponent::update(float currentTime, float elapsedTime)
{
	if (followsPath())
	{
		flat::Vector2& pathNextPoint = m_path.front();
		const flat::Vector3& position = m_owner->getPosition();
		flat::Vector2 position2d = flat::Vector2(position.x, position.y);
		flat::Vector2 move = pathNextPoint - position2d;
		if (flat::length2(move) > 0.f)
		{
			m_owner->setHeading(flat::vector2_angle(move));
			flat::Vector2 direction = normalize(move);
			const MovementComponentTemplate* movementComponentTemplate = getTemplate();
			const float speed = movementComponentTemplate->getSpeed();
			flat::Vector2 newPosition2d = position2d + direction * speed * elapsedTime;
			
			const map::Map* map = m_owner->getMap();
			flat::Vector2 nextTilePosition = position2d + direction * 0.4f;
			const map::Tile* nextTile = map->getTileIfWalkable(nextTilePosition.x, nextTilePosition.y);
			
			if (nextTile && (nextTile->getZ() > position.z + MIN_Z_EPSILON || nextTile->getZ() < position.z - MIN_Z_EPSILON))
			{
				jump();
			}
			
			flat::Vector2 newMove = pathNextPoint - newPosition2d;
			if (dot(move, newMove) <= 0.f)
			{
				newPosition2d = pathNextPoint;
				m_path.pop();
				if (!followsPath())
				{
					movementStopped();
				}
			}
			m_owner->setXY(newPosition2d);
		}
	}
	
	fall(elapsedTime);
}

void MovementComponent::addedToMap(map::Map* map)
{
	const map::Tile* tile = m_owner->getTile();
	flat::Vector3 position = m_owner->getPosition();
	if (tile->getZ() >= position.z)
	{
		m_isTouchingGround = true;
		m_owner->setZ(tile->getZ());
	}
	else
	{
		m_isTouchingGround = false;
	}
	m_zSpeed = 0.f;
}

bool MovementComponent::isBusy() const
{
	return followsPath() || !isTouchingGround();
}

bool MovementComponent::followsPath() const
{
	return !m_path.empty();
}

void MovementComponent::addPointOnPath(const flat::Vector2& point)
{
	bool startMovement = false;
	flat::Vector2 startingPoint;
	if (m_path.empty())
	{
		const flat::Vector3& position = m_owner->getPosition();
		startingPoint.x = position.x;
		startingPoint.y = position.y;
		startMovement = true;
	}
	else
	{
		startingPoint = m_path.back();
	}
	
	const float minDistanceBetweenPoints = 0.3f;
	if (flat::length2(point - startingPoint) > minDistanceBetweenPoints * minDistanceBetweenPoints)
	{
		const map::Map& map = *m_owner->getMap();
		const float jumpHeight = getTemplate()->getJumpMaxHeight();
		map::Pathfinder pathfinder(map, jumpHeight);
		std::vector<flat::Vector2> path;
		if (pathfinder.findPath(startingPoint, point, path))
		{
			path.erase(path.begin());
			for (const flat::Vector2& point : path)
			{
				m_path.push(point);
			}
		}
		else
		{
			m_path.push(point);
		}
		
		if (startMovement)
		{
			movementStarted();
		}
	}
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

} // component
} // entity
} // game



