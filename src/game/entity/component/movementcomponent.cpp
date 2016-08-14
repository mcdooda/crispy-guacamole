#include "movementcomponent.h"
#include "../entity.h"
#include "../entitytemplate.h"
#include "../../map/map.h"
#include "../../map/tile.h"

namespace game
{
namespace entity
{
namespace component
{

void MovementComponent::update(float currentTime, float elapsedTime)
{
	if (!followsPath())
		return;
	
	flat::geometry::Vector2& pathNextPoint = m_path.front();
	const flat::geometry::Vector3& position = m_owner->getPosition();
	flat::geometry::Vector2 position2d = flat::geometry::Vector2(position.x, position.y);
	flat::geometry::Vector2 move = pathNextPoint - position2d;
	if (move.lengthSquared() > 0.f)
	{
		m_owner->setHeading(move.angle());
		flat::geometry::Vector2 direction = move.normalize();
		const EntityTemplate* entityTemplatePtr = m_owner->getEntityTemplate().get();
		float speed = entityTemplatePtr->getSpeed();
		flat::geometry::Vector2 newPosition2d = position2d + direction * speed * elapsedTime;
		flat::geometry::Vector2 newMove = pathNextPoint - newPosition2d;
		if (move.dotProduct(newMove) < 0.f)
		{
			newPosition2d = pathNextPoint;
			m_path.pop();
			if (!followsPath())
			{
				m_owner->movementStopped();
			}
		}
		m_owner->setPosition(flat::geometry::Vector3(newPosition2d.x, newPosition2d.y, position.z));
	}
	
	separateFromAdjacentTiles();
}

bool MovementComponent::isBusy() const
{
	return followsPath();
}

bool MovementComponent::followsPath() const
{
	return !m_path.empty();
}

void MovementComponent::addPointOnPath(const flat::geometry::Vector2& point)
{
	const float minDistanceBetweenPoints = 0.3f;
	if (m_path.empty())
	{
		const flat::geometry::Vector3& position = m_owner->getPosition();
		flat::geometry::Vector2 position2d(position.x, position.y);
		if ((point - position2d).lengthSquared() > minDistanceBetweenPoints * minDistanceBetweenPoints)
		{
			m_owner->movementStarted();
			m_path.push(point);
		}
	}
	else
	{
		const flat::geometry::Vector2& lastPoint = m_path.back();
		if ((point - lastPoint).lengthSquared() > minDistanceBetweenPoints * minDistanceBetweenPoints)
		{
			m_path.push(point);
		}
	}
}

void MovementComponent::separateFromAdjacentTiles()
{
	const map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map);
	const map::Tile* tile = m_owner->getTile();
	FLAT_ASSERT(tile);
	const int tileX = static_cast<float>(tile->getX());
	const int tileY = static_cast<float>(tile->getY());
	
	const flat::geometry::Vector3& position = m_owner->getPosition();
	const float z = position.z;
	FLAT_ASSERT(z >= tile->getZ());
	
	flat::geometry::Vector2 newPosition2d(position.x, position.y);
	
	const EntityTemplate* entityTemplatePtr = m_owner->getEntityTemplate().get();
	const float radius = entityTemplatePtr->getRadius();
	
	// directly adjacent tiles
	// top right
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX - 1, tileY);
		if (!tile2 || tile2->getZ() > z)
		{
			newPosition2d.x = std::max(newPosition2d.x, static_cast<float>(tileX) - 0.5f + radius);
		}
	}
	// bottom left
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX + 1, tileY);
		if (!tile2 || tile2->getZ() > z)
		{
			newPosition2d.x = std::min(newPosition2d.x, static_cast<float>(tileX) + 0.5f - radius);
		}
	}
	// top left
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX, tileY - 1);
		if (!tile2 || tile2->getZ() > z)
		{
			newPosition2d.y = std::max(newPosition2d.y, static_cast<float>(tileY) - 0.5f + radius);
		}
	}
	// bottom right
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX, tileY + 1);
		if (!tile2 || tile2->getZ() > z)
		{
			newPosition2d.y = std::min(newPosition2d.y, static_cast<float>(tileY) + 0.5f - radius);
		}
	}
	
	// diagonals
	// top
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX - 1, tileY - 1);
		if (!tile2 || tile2->getZ() > z)
		{
			flat::geometry::Vector2 corner(static_cast<float>(tileX) - 0.5f, static_cast<float>(tileY) - 0.5f);
			flat::geometry::Vector2 toCorner = corner - newPosition2d;
			if (toCorner.lengthSquared() < radius * radius)
			{
				newPosition2d = corner - toCorner.normalize() * radius;
			}
		}
	}
	// bottom
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX + 1, tileY + 1);
		if (!tile2 || tile2->getZ() > z)
		{
			flat::geometry::Vector2 corner(static_cast<float>(tileX) + 0.5f, static_cast<float>(tileY) + 0.5f);
			flat::geometry::Vector2 toCorner = corner - newPosition2d;
			if (toCorner.lengthSquared() < radius * radius)
			{
				newPosition2d = corner - toCorner.normalize() * radius;
			}
		}
	}
	// left
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX + 1, tileY - 1);
		if (!tile2 || tile2->getZ() > z)
		{
			flat::geometry::Vector2 corner(static_cast<float>(tileX) + 0.5f, static_cast<float>(tileY) - 0.5f);
			flat::geometry::Vector2 toCorner = corner - newPosition2d;
			if (toCorner.lengthSquared() < radius * radius)
			{
				newPosition2d = corner - toCorner.normalize() * radius;
			}
		}
	}
	// right
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX - 1, tileY + 1);
		if (!tile2 || tile2->getZ() > z)
		{
			flat::geometry::Vector2 corner(static_cast<float>(tileX) - 0.5f, static_cast<float>(tileY) + 0.5f);
			flat::geometry::Vector2 toCorner = corner - newPosition2d;
			if (toCorner.lengthSquared() < radius * radius)
			{
				newPosition2d = corner - toCorner.normalize() * radius;
			}
		}
	}
	
	m_owner->setPosition(flat::geometry::Vector3(newPosition2d.x, newPosition2d.y, z));
}

} // component
} // entity
} // game



