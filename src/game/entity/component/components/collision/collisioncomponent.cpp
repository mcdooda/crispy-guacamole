#include "collisioncomponent.h"
#include "collisioncomponenttemplate.h"
#include "../movement/movementcomponent.h"
#include "../../componenttype.h"
#include "../../../entity.h"
#include "../../../entitytemplate.h"
#include "../../../../map/map.h"
#include "../../../../map/tile.h"

namespace game
{
namespace entity
{
namespace component
{

void CollisionComponent::update(float currentTime, float elapsedTime)
{
	separateFromNearbyEntities();
	separateFromAdjacentTiles();
}

void CollisionComponent::separateFromAdjacentTiles()
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
	
	const float radius = getTemplate()->getRadius();
	
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
	
	if (position.x != newPosition2d.x || position.y != newPosition2d.y)
	{
		m_owner->setXY(newPosition2d);
		onCollidedWithMap();
	}
}

void CollisionComponent::separateFromNearbyEntities()
{
	const flat::Vector3& position = m_owner->getPosition();
	const CollisionComponentTemplate* collisionComponentTemplate = getTemplate();
	const MovementComponentTemplate* movementComponentTemplate = getTemplate<MovementComponent>();
	const float weight = movementComponentTemplate ? movementComponentTemplate->getWeight() : 0.f;
	const CollisionBox& collisionBox = collisionComponentTemplate->getCollisionBox();
	const float maxEntityRadius = 0.5f;
	const float radius = collisionComponentTemplate->getRadius();
	FLAT_ASSERT(radius <= maxEntityRadius);
	const int tileMinX = static_cast<int>(std::round(position.x - radius - maxEntityRadius));
	const int tileMinY = static_cast<int>(std::round(position.y - radius - maxEntityRadius));
	const int tileMaxX = static_cast<int>(std::round(position.x + radius + maxEntityRadius));
	const int tileMaxY = static_cast<int>(std::round(position.y + radius + maxEntityRadius));
	const map::Map* map = m_owner->getMap();
	flat::Vector3 newPosition = position;
	for (int x = tileMinX; x <= tileMaxX; ++x)
	{
		for (int y = tileMinY; y <= tileMaxY; ++y)
		{
			const map::Tile* tile = map->getTileIfWalkable(x, y);
			if (tile)
			{
				// we actually need to copy this as it is iterated and modified at the same type
				const std::vector<Entity*> neighbors = tile->getEntities();
				for (Entity* neighbor : neighbors)
				{
					if (neighbor == m_owner)
						continue;
					
					const flat::Vector3& neighborPosition = neighbor->getPosition();
					const EntityTemplate* neighborTemplate = neighbor->getEntityTemplate().get();
					const CollisionComponentTemplate* neighborCollisionComponentTemplate = neighborTemplate->getComponentTemplate<CollisionComponent>();
					if (neighborCollisionComponentTemplate != nullptr)
					{
						const CollisionBox& neighborCollisionBox = neighborCollisionComponentTemplate->getCollisionBox();
						const EntityTemplate* neighborEntityTemplate = neighbor->getEntityTemplate().get();
						flat::Vector3 distanceVec = neighborPosition - position;
						float distance = flat::length(distanceVec);
						flat::Vector3 penetration;
						if (CollisionBox::collides(position, neighborPosition, collisionBox, neighborCollisionBox, penetration))
						{
							onCollidedWithEntity(neighbor);
							const MovementComponentTemplate* neighborMovementComponentTemplate = neighborTemplate->getComponentTemplate<MovementComponent>();
							const float neighborWeight = neighborMovementComponentTemplate ? neighborMovementComponentTemplate->getWeight() : 0.f;
							if (neighborWeight + weight > 0.f)
							{
								const float neighborMoveRatio = neighborWeight / (neighborWeight + weight);
								neighbor->setPosition(neighborPosition + penetration * neighborMoveRatio);
								const float moveRatio = neighborMoveRatio - 1.f;
								newPosition += penetration * moveRatio;
							}
						}
					}
				}
			}
		}
	}
	m_owner->setPosition(newPosition);
}

} // component
} // entity
} // game



