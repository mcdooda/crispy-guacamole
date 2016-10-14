#include "movementcomponent.h"
#include "componenttype.h"
#include "../entity.h"
#include "../entitytemplate.h"
#include "../../map/map.h"
#include "../../map/tile.h"
#include "../../map/pathfinder.h"

namespace game
{
namespace entity
{
namespace component
{

DEFINE_COMPONENT_TYPE(MovementComponent)

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
			const EntityTemplate* entityTemplatePtr = m_owner->getEntityTemplate().get();
			float speed = entityTemplatePtr->getSpeed();
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
	separateFromNearbyEntities();
	separateFromAdjacentTiles();
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
		const float jumpHeight = m_owner->getEntityTemplate()->getJumpMaxHeight();
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
		m_zSpeed = m_owner->getEntityTemplate()->getJumpForce();
		m_isTouchingGround = false;
	}
}

void MovementComponent::fall(float elapsedTime)
{
	if (m_isTouchingGround)
		return;
		
	const map::Tile* tile = m_owner->getTile();
	const float acceleration = m_owner->getEntityTemplate()->getWeight();
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

void MovementComponent::separateFromAdjacentTiles()
{
	const map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map != nullptr);
	const map::Tile* tile = m_owner->getTile();
	FLAT_ASSERT(tile != nullptr);
	const int tileX = tile->getX();
	const int tileY = tile->getY();
	
	const flat::Vector3& position = m_owner->getPosition();
	const float z = position.z;
	const float minZ = z + MIN_Z_EPSILON;
	//FLAT_ASSERT(minZ >= tile->getZ());
	
	flat::Vector2 newPosition2d(position.x, position.y);
	
	const EntityTemplate* entityTemplatePtr = m_owner->getEntityTemplate().get();
	const float radius = entityTemplatePtr->getRadius();
	
	// directly adjacent tiles
	// top right
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX - 1, tileY);
		if (!tile2 || tile2->getZ() > minZ)
		{
			newPosition2d.x = std::max(newPosition2d.x, static_cast<float>(tileX) - 0.5f + radius);
		}
	}
	// bottom left
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX + 1, tileY);
		if (!tile2 || tile2->getZ() > minZ)
		{
			newPosition2d.x = std::min(newPosition2d.x, static_cast<float>(tileX) + 0.5f - radius);
		}
	}
	// top left
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX, tileY - 1);
		if (!tile2 || tile2->getZ() > minZ)
		{
			newPosition2d.y = std::max(newPosition2d.y, static_cast<float>(tileY) - 0.5f + radius);
		}
	}
	// bottom right
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX, tileY + 1);
		if (!tile2 || tile2->getZ() > minZ)
		{
			newPosition2d.y = std::min(newPosition2d.y, static_cast<float>(tileY) + 0.5f - radius);
		}
	}
	
	// diagonals
	// top
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX - 1, tileY - 1);
		if (!tile2 || tile2->getZ() > minZ)
		{
			flat::Vector2 corner(static_cast<float>(tileX) - 0.5f, static_cast<float>(tileY) - 0.5f);
			flat::Vector2 toCorner = corner - newPosition2d;
			if (flat::length2(toCorner) < radius * radius)
			{
				newPosition2d = corner - flat::normalize(toCorner) * radius;
			}
		}
	}
	// bottom
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX + 1, tileY + 1);
		if (!tile2 || tile2->getZ() > minZ)
		{
			flat::Vector2 corner(static_cast<float>(tileX) + 0.5f, static_cast<float>(tileY) + 0.5f);
			flat::Vector2 toCorner = corner - newPosition2d;
			if (flat::length2(toCorner) < radius * radius)
			{
				newPosition2d = corner - flat::normalize(toCorner) * radius;
			}
		}
	}
	// left
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX + 1, tileY - 1);
		if (!tile2 || tile2->getZ() > minZ)
		{
			flat::Vector2 corner(static_cast<float>(tileX) + 0.5f, static_cast<float>(tileY) - 0.5f);
			flat::Vector2 toCorner = corner - newPosition2d;
			if (flat::length2(toCorner) < radius * radius)
			{
				newPosition2d = corner - flat::normalize(toCorner) * radius;
			}
		}
	}
	// right
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX - 1, tileY + 1);
		if (!tile2 || tile2->getZ() > minZ)
		{
			flat::Vector2 corner(static_cast<float>(tileX) - 0.5f, static_cast<float>(tileY) + 0.5f);
			flat::Vector2 toCorner = corner - newPosition2d;
			if (flat::length2(toCorner) < radius * radius)
			{
				newPosition2d = corner - flat::normalize(toCorner) * radius;
			}
		}
	}
	
	m_owner->setXY(newPosition2d);
}

void MovementComponent::separateFromNearbyEntities()
{
	const flat::Vector3& position = m_owner->getPosition();
	const EntityTemplate* entityTemplate = m_owner->getEntityTemplate().get();
	const float radius = entityTemplate->getRadius();
	const float weight = entityTemplate->getWeight();
	const float maxEntityRadius = 0.5f;
	const int tileMinX = static_cast<int>(std::round(position.x - radius - maxEntityRadius));
	const int tileMinY = static_cast<int>(std::round(position.y - radius - maxEntityRadius));
	const int tileMaxX = static_cast<int>(std::round(position.x + radius + maxEntityRadius));
	const int tileMaxY = static_cast<int>(std::round(position.y + radius + maxEntityRadius));
	const map::Map* map = m_owner->getMap();
	flat::Vector2 position2d(position.x, position.y);
	for (int x = tileMinX; x <= tileMaxX; ++x)
	{
		for (int y = tileMinY; y <= tileMaxY; ++y)
		{
			const map::Tile* tile = map->getTileIfWalkable(x, y);
			if (tile)
			{
				// we actually need to copy this as it is iterated and modified at the same type
				const std::vector<entity::Entity*> neighbors = tile->getEntities();
				for (entity::Entity* neighbor : neighbors)
				{
					if (neighbor == m_owner)
						continue;
					
					const flat::Vector3& neighborPosition = neighbor->getPosition();
					const EntityTemplate* neighborEntityTemplate = neighbor->getEntityTemplate().get();
					const float neighborRadius = neighborEntityTemplate->getRadius();
					flat::Vector2 neighborPosition2d(neighborPosition.x, neighborPosition.y);
					const float minDistance = radius + neighborRadius - 0.001f;
					if (flat::length2(neighborPosition2d - position2d) < minDistance * minDistance)
					{
						const float penetration = -(flat::length(neighborPosition2d - position2d) - radius - neighborRadius);
						const float neighborWeight = neighborEntityTemplate->getWeight();
						const float neighborMoveRatio = neighborWeight / (neighborWeight + weight);
						flat::Vector2 neighborMove = flat::normalize(neighborPosition2d - position2d);
						neighborPosition2d += neighborMove * penetration * neighborMoveRatio;
						neighbor->setXY(neighborPosition2d);
						
						const float moveRatio = weight / (neighborWeight + weight);
						position2d += -neighborMove * penetration * moveRatio;
					}
				}
			}
		}
	}
	m_owner->setXY(position2d);
}

} // component
} // entity
} // game



