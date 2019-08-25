#include "collisioncomponent.h"
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

float CollisionComponent::getBottom(float z) const
{
	const CollisionComponentTemplate* collisionComponentTemplate = getTemplate();
	const CollisionBox& collisionBox = collisionComponentTemplate->getCollisionBox();
	return collisionBox.getBottom(z);
}

void CollisionComponent::separateFromNearbyEntities()
{
	const CollisionComponentTemplate* collisionComponentTemplate = getTemplate();
	if (!collisionComponentTemplate->shouldSeparateFromOtherEntities())
		return;

	const flat::Vector3& position = m_owner->getPosition();
	const movement::MovementComponentTemplate* movementComponentTemplate = getTemplate<movement::MovementComponent>();
	const float weight = movementComponentTemplate ? movementComponentTemplate->getWeight() : 0.f;
	const CollisionBox& collisionBox = collisionComponentTemplate->getCollisionBox();
	const float radius = collisionComponentTemplate->getRadius();
	const map::Map* map = m_owner->getMap();
	flat::Vector3 newPosition = position;

	std::vector<entity::Entity*> neighbors;
	neighbors.reserve(16);
	map->eachEntityInCollisionRange(
		flat::Vector2(position),
		radius,
		[this, &neighbors](entity::Entity* neighbor)
		{
			if (neighbor == m_owner)
				return;

			neighbors.push_back(neighbor);
		}
	);

	for (entity::Entity* neighbor : neighbors)
	{
		const flat::Vector3& neighborPosition = neighbor->getPosition();
		const EntityTemplate* neighborTemplate = neighbor->getEntityTemplate().get();
		const CollisionComponentTemplate* neighborCollisionComponentTemplate = neighborTemplate->getComponentTemplate<CollisionComponent>();
		if (neighborCollisionComponentTemplate != nullptr && neighborCollisionComponentTemplate->shouldSeparateFromOtherEntities()
			&& !(neighborCollisionComponentTemplate == collisionComponentTemplate && !collisionComponentTemplate->shouldSeparateFromSameType()))
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
					const float moveRatio = neighborMoveRatio - 1.f;
					newPosition += penetration * moveRatio;
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

	map::TileIndex tileIndex = m_owner->getTileIndexFromPosition();
	FLAT_ASSERT(tileIndex != map::TileIndex::INVALID_TILE);
	flat::Vector2i tilePosition = map->getTileXY(tileIndex);
	
	const flat::Vector3& position = m_owner->getPosition();
	
	flat::Vector3 newPosition = position;
	
	const CollisionComponentTemplate* collisionComponentTemplate = getTemplate();
	const float radius = collisionComponentTemplate->getRadius();
	const float radius2 = flat::square(radius);

	map::Navigability navigabilityMask = EntityHelper::getNavigabilityMask(m_owner);

	const int minX = static_cast<int>(std::round(m_owner->getPosition().x - radius));
	const int maxX = static_cast<int>(std::round(m_owner->getPosition().x + radius));
	const int minY = static_cast<int>(std::round(m_owner->getPosition().y - radius));
	const int maxY = static_cast<int>(std::round(m_owner->getPosition().y + radius));

	map::TileIndex collidedTileIndex = map::TileIndex::INVALID_TILE;
	float collidedTileZ = -FLT_MAX;
	flat::Vector3 normal;
	float closestCollisionDistance2 = FLT_MAX;

	auto computeTileCollision = [&](int x, int y)
	{
		map::TileIndex tileIndex2 = map->getTileIndexIfNavigable(x, y, navigabilityMask);
		if (tileIndex2 == tileIndex)
		{
			const float bottom = getBottom(newPosition.z);
			const float tileZ = map->getTileZ(tileIndex);
			if (bottom < tileZ && bottom > tileZ - 0.5f)
			{
				newPosition.z = tileZ;
				collidedTileIndex = tileIndex;
				normal = flat::Vector3(0.f, 0.f, 1.f);
				return;
			}
		}
		
		float tileZ = -FLT_MAX;
		if (tileIndex2 != map::TileIndex::INVALID_TILE && map->getTileZ(tileIndex2) < getBottom(newPosition.z) + MIN_Z_EPSILON)
		{
			return;
		}

		// find collision point
		flat::Vector3 closestPointOnEdge;
		closestPointOnEdge.z = newPosition.z;
		const flat::Vector2 locationOnTile(newPosition.x - static_cast<float>(x), newPosition.y - static_cast<float>(y));

		if (locationOnTile.y < locationOnTile.x) // bottom and right
		{
			if (locationOnTile.y < -locationOnTile.x) // bottom
			{
				closestPointOnEdge.x = std::clamp(newPosition.x, static_cast<float>(x) - 0.5f, static_cast<float>(x) + 0.5f);
				closestPointOnEdge.y = static_cast<float>(y) - 0.5f;
			}
			else // right
			{
				closestPointOnEdge.x = static_cast<float>(x) + 0.5f;
				closestPointOnEdge.y = std::clamp(newPosition.y, static_cast<float>(y) - 0.5f, static_cast<float>(y) + 0.5f);
			}
		}
		else // top and left
		{
			if (locationOnTile.y > -locationOnTile.x) // top
			{
				closestPointOnEdge.x = std::clamp(newPosition.x, static_cast<float>(x) - 0.5f, static_cast<float>(x) + 0.5f);
				closestPointOnEdge.y = static_cast<float>(y) + 0.5f;
			}
			else // left
			{
				closestPointOnEdge.x = static_cast<float>(x) - 0.5f;
				closestPointOnEdge.y = std::clamp(newPosition.y, static_cast<float>(y) - 0.5f, static_cast<float>(y) + 0.5f);
			}
		}

		// deduce collision normal and new position
		float distance2;
		if (std::abs(locationOnTile.x) < 0.5f && std::abs(locationOnTile.y) < 0.5f)
		{
			distance2 = 0.f;
		}
		else
		{
			distance2 = flat::distance2(newPosition, closestPointOnEdge);
		}

		if (distance2 < radius2)
		{
			if (tileIndex2 != map::TileIndex::INVALID_TILE)
			{
				const float distance = std::sqrt(distance2);
				if (distance < radius - 1.f && tileZ > newPosition.z)
				{
					normal = flat::Vector3(0.f, 0.f, 1.f);
					newPosition.z = tileZ;
					collidedTileIndex = tileIndex2;
					return;
				}
			}

			if (distance2 < closestCollisionDistance2)
			{
				closestCollisionDistance2 = distance2;

				const flat::Vector3 tileCenter(x, y, newPosition.z);
				normal = flat::normalize(newPosition - closestPointOnEdge);

				newPosition = closestPointOnEdge + normal * radius;
				collidedTileIndex = tileIndex2;
			}
		}
	};

	for (int x = minX; x <= maxX; ++x)
	{
		for (int y = minY; y <= maxY; ++y)
		{
			computeTileCollision(x, y);
		}
	}

	if (position != newPosition)
	{
		const map::TileIndex newTileIndex = map->getTileIndex(newPosition.x, newPosition.y);
		if (newTileIndex != map::TileIndex::INVALID_TILE)
		{
			const float newTileZ = map->getTileZ(newTileIndex);
			newPosition.z = std::max(newPosition.z, newTileZ);
		}

		m_owner->setPosition(newPosition);
		FLAT_ASSERT(flat::length2(normal) > 0.f || collidedTileIndex == map::TileIndex::INVALID_TILE);
		onCollidedWithMap(collidedTileIndex, normal);
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

		const float r = radius * flat::SQRT_2 / 2.f;

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



