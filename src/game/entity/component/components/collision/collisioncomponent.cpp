#include "collisioncomponent.h"
#include "collisioncomponenttemplate.h"
#include "../movement/movementcomponent.h"
#include "../../componenttype.h"
#include "../../../entity.h"
#include "../../../entitytemplate.h"
#include "../../../entityhelper.h"
#include "../../../../map/map.h"
#include "../../../../map/tile.h"

namespace game
{
namespace entity
{
namespace component
{
namespace collision
{

void CollisionComponent::update(float currentTime, float elapsedTime)
{
	separateFromNearbyEntities();
	separateFromAdjacentTiles();
}

void CollisionComponent::getAABB(flat::AABB3& aabb) const
{
	const CollisionComponentTemplate* collisionComponentTemplate = getTemplate();
	const CollisionBox& collisionBox = collisionComponentTemplate->getCollisionBox();
	collisionBox.getAABB(m_owner->getPosition(), aabb);
}

float CollisionComponent::getBottom() const
{
	const CollisionComponentTemplate* collisionComponentTemplate = getTemplate();
	const CollisionBox& collisionBox = collisionComponentTemplate->getCollisionBox();
	return collisionBox.getBottom(m_owner->getPosition());
}

void CollisionComponent::separateFromNearbyEntities()
{
	const CollisionComponentTemplate* collisionComponentTemplate = getTemplate();
	if (!collisionComponentTemplate->getSeparate())
		return;

	const flat::Vector3& position = m_owner->getPosition();
	const movement::MovementComponentTemplate* movementComponentTemplate = getTemplate<movement::MovementComponent>();
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
	// we can't use Map::eachEntityInRange here because we make changes to Tile::m_entities while iterating it
	for (int x = tileMinX; x <= tileMaxX; ++x)
	{
		for (int y = tileMinY; y <= tileMaxY; ++y)
		{
			const map::Tile* tile = map->getTileIfWalkable(x, y);
			if (tile)
			{
				// we actually need to copy this as it is iterated and modified at the same time
				const std::vector<Entity*> neighbors = tile->getEntities();
				for (Entity* neighbor : neighbors)
				{
					if (neighbor == m_owner)
						continue;

					const flat::Vector3& neighborPosition = neighbor->getPosition();
					const EntityTemplate* neighborTemplate = neighbor->getEntityTemplate().get();
					const CollisionComponentTemplate* neighborCollisionComponentTemplate = neighborTemplate->getComponentTemplate<CollisionComponent>();
					if (neighborCollisionComponentTemplate != nullptr && neighborCollisionComponentTemplate->getSeparate())
					{
						const CollisionBox& neighborCollisionBox = neighborCollisionComponentTemplate->getCollisionBox();
						flat::Vector3 penetration;
						if (CollisionBox::collides(position, neighborPosition, collisionBox, neighborCollisionBox, penetration))
						{
							flat::Vector3 normal = flat::normalize(penetration);
							onCollidedWithEntity(neighbor, normal);
							const movement::MovementComponentTemplate* neighborMovementComponentTemplate = neighborTemplate->getComponentTemplate<movement::MovementComponent>();
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
	if (newPosition != position)
	{
		m_owner->setPosition(newPosition);
	}
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
	const float minZ = position.z + MIN_Z_EPSILON;
	
	flat::Vector2 newPosition2d(position.x, position.y);
	
	const CollisionComponentTemplate* collisionComponentTemplate = getTemplate();
	const float radius = collisionComponentTemplate->getRadius();
	
	const map::Tile* collidedTile = nullptr;
	flat::Vector3 normal;

	// directly adjacent tiles
	// top right
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX - 1, tileY);
		if (!tile2 || tile2->getZ() > minZ)
		{
			newPosition2d.x = std::max(newPosition2d.x, static_cast<float>(tileX) - 0.5f + radius);
			collidedTile = tile2;
			normal = flat::Vector3(1.f, 0.f, 0.f);
		}
	}
	// bottom left
	if (collidedTile == nullptr)
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX + 1, tileY);
		if (!tile2 || tile2->getZ() > minZ)
		{
			newPosition2d.x = std::min(newPosition2d.x, static_cast<float>(tileX) + 0.5f - radius);
			collidedTile = tile2;
			normal = flat::Vector3(-1.f, 0.f, 0.f);
		}
	}
	// top left
	if (collidedTile == nullptr)
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX, tileY - 1);
		if (!tile2 || tile2->getZ() > minZ)
		{
			newPosition2d.y = std::max(newPosition2d.y, static_cast<float>(tileY) - 0.5f + radius);
			collidedTile = tile2;
			normal = flat::Vector3(0.f, -1.f, 0.f);
		}
	}
	// bottom right
	if (collidedTile == nullptr)
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX, tileY + 1);
		if (!tile2 || tile2->getZ() > minZ)
		{
			newPosition2d.y = std::min(newPosition2d.y, static_cast<float>(tileY) + 0.5f - radius);
			collidedTile = tile2;
			normal = flat::Vector3(0.f, 1.f, 0.f);
		}
	}
	
	// diagonals
	// top
	if (collidedTile == nullptr)
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
			collidedTile = tile2;
			normal = flat::Vector3(-flat::SQRT2 / 2.f, -flat::SQRT2 / 2.f, 0.f);
		}
	}
	// bottom
	if (collidedTile == nullptr)
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
			collidedTile = tile2;
			normal = flat::Vector3(flat::SQRT2 / 2.f, flat::SQRT2 / 2.f, 0.f);
		}
	}
	// left
	if (collidedTile == nullptr)
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
			collidedTile = tile2;
			normal = flat::Vector3(-flat::SQRT2 / 2.f, flat::SQRT2 / 2.f, 0.f);
		}
	}
	// right
	if (collidedTile == nullptr)
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
			collidedTile = tile2;
			normal = flat::Vector3(flat::SQRT2 / 2.f, -flat::SQRT2 / 2.f, 0.f);
		}
	}
	
	flat::Vector3 newPosition(newPosition2d, position.z);

	// check collision with the current tile
	const float tileZ = tile->getZ();
	const float bottom = getBottom();
	bool collidedWithCurrentTile = false;
	flat::Vector3 currentTileCollisionNormal;
	if (bottom < tileZ)
	{
		collidedWithCurrentTile = true;
		if (bottom > tileZ - 0.5f)
		{
			newPosition.z += tileZ - bottom;
			currentTileCollisionNormal = flat::Vector3(0.f, 0.f, 1.f);
		}
		else
		{
			currentTileCollisionNormal.z = 0.f;

			const float tileX = static_cast<float>(tile->getX());
			const float tileY = static_cast<float>(tile->getY());

			if (std::abs(tileX - newPosition.x) > std::abs(tileY - newPosition.y))
			{
				if (tileX < newPosition.x)
				{
					newPosition.x = tileX + 0.5f + radius;
					currentTileCollisionNormal.x = 1.f;
				}
				else
				{
					newPosition.x = tileX - 0.5f - radius;
					currentTileCollisionNormal.x = -1.f;
				}
			}
			else
			{
				if (tileY < newPosition.y)
				{
					newPosition.y = tileY + 0.5f + radius;
					currentTileCollisionNormal.y = 1.f;
				}
				else
				{
					newPosition.y = tileY - 0.5f - radius;
					currentTileCollisionNormal.y = -1.f;
				}
			}

			const map::Tile* newTile = map->getTileIfExists(newPosition.x, newPosition.y);
			if (newTile != nullptr)
			{
				const float newTileZ = newTile->getZ();
				if (bottom < newTileZ)
				{
					newPosition.z += newTileZ - bottom;
				}
			}

			currentTileCollisionNormal = flat::normalize(currentTileCollisionNormal);
		}
	}

	if (position != newPosition)
	{
		m_owner->setPosition(newPosition);
		FLAT_ASSERT(flat::length2(normal) > 0.f || collidedTile == nullptr);
		if (collidedTile == nullptr && !collidedWithCurrentTile) // collided with the border of the map
		{
			onCollidedWithMap(nullptr, normal);
		}
		else
		{
			if (collidedTile != nullptr && collidedTile->getZ() >= minZ) // collided with an adjacent tile
			{
				onCollidedWithMap(collidedTile, normal);
			}
			if (collidedWithCurrentTile) // collided with the tile the entity is already above
			{
				onCollidedWithMap(tile, currentTileCollisionNormal);
			}
		}
	}
}

#ifdef FLAT_DEBUG
void CollisionComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	const CollisionComponentTemplate* collisionComponentTemplate = getTemplate();
	const CollisionBox& collisionBox = collisionComponentTemplate->getCollisionBox();
	
	const float radius = collisionBox.getRadius();
	debugDisplay.add3dCircle(m_owner->getPosition(), radius, flat::video::Color::BLUE, 0.5f);

	if (collisionBox.type == CollisionType::CYLINDER)
	{
		debugDisplay.add3dCircle(
			m_owner->getPosition() + flat::Vector3(0.f, 0.f, collisionBox.cylinder.height),
			collisionBox.getRadius(),
			flat::video::Color::BLUE,
			0.5f
		);

		const float r = radius * flat::SQRT2 / 2.f;

		// left
		debugDisplay.add3dLine(
			m_owner->getPosition() + flat::Vector3(-r, r, 0.f),
			m_owner->getPosition() + flat::Vector3(-r, r, collisionBox.cylinder.height),
			flat::video::Color::BLUE,
			0.5f
		);

		// right
		debugDisplay.add3dLine(
			m_owner->getPosition() + flat::Vector3(r, -r, 0.f),
			m_owner->getPosition() + flat::Vector3(r, -r, collisionBox.cylinder.height),
			flat::video::Color::BLUE,
			0.5f
		);
	}
}
#endif

} // collision
} // component
} // entity
} // game



