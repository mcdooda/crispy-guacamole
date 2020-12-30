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

flat::Vector3 CollisionComponent::getCenter() const
{
	const CollisionComponentTemplate* collisionComponentTemplate = getTemplate();
	const CollisionBox& collisionBox = collisionComponentTemplate->getCollisionBox();
	return collisionBox.getCenter(m_owner->getPosition());
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
		m_owner->setPositionSweep(newPosition);
	}
}

void CollisionComponent::separateFromAdjacentTiles()
{
	const CollisionComponentTemplate* collisionComponentTemplate = getTemplate();
	if (!collisionComponentTemplate->shouldSeparateFromTiles())
		return;

	const map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map != nullptr);

	map::TileIndex tileIndex = m_owner->getTileIndexFromPosition();
	FLAT_ASSERT(map::isValidTile(tileIndex));
	
	const flat::Vector3& position = m_owner->getPosition();
	const flat::Vector2& position2d = m_owner->getPosition2d();
	
	flat::Vector3 newPosition = position;
	//FLAT_DEBUG_ONLY(m_owner->checkValidPosition(newPosition));
	
	const float radius = collisionComponentTemplate->getRadius();
	const float radius2 = flat::square(radius);

	map::Navigability navigabilityMask = EntityHelper::getNavigabilityMask(m_owner);

	const int minX = static_cast<int>(std::round(m_owner->getPosition().x - radius));
	const int maxX = static_cast<int>(std::round(m_owner->getPosition().x + radius));
	const int minY = static_cast<int>(std::round(m_owner->getPosition().y - radius));
	const int maxY = static_cast<int>(std::round(m_owner->getPosition().y + radius));

	map::TileIndex collidedTileIndex = map::TileIndex::INVALID_VALUE;
	float collidedTileZ = -FLT_MAX;
	flat::Vector3 normal;
	float closestCollisionDistance = FLT_MAX;

	auto computeTileCollision = [&](int x, int y)
	{
		const map::TileIndex tileIndex2 = map->findTileIndexIfNavigable(flat::Vector2(x, y), navigabilityMask);
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
		const bool tileIsValid = map::isValidTile(tileIndex2);

		if (tileIsValid)
		{
			tileZ = map->getTileZ(tileIndex2);
			if (tileZ < getBottom(newPosition.z) + Entity::MIN_Z_EPSILON)
			{
				return;
			}
		}

		flat::AABB2 tileAABB2;
		if (tileIsValid)
		{
			const flat::AABB3& tileAABB3 = map->getTileAABB(tileIndex2);
			tileAABB2.min.x = tileAABB3.min.x;
			tileAABB2.min.y = tileAABB3.min.y;
			tileAABB2.max.x = tileAABB3.max.x;
			tileAABB2.max.y = tileAABB3.max.y;
		}
		else
		{
			tileAABB2.min.x = static_cast<float>(x) - 0.5f;
			tileAABB2.min.y = static_cast<float>(y) - 0.5f;
			tileAABB2.max.x = static_cast<float>(x) + 0.5f;
			tileAABB2.max.y = static_cast<float>(y) + 0.5f;
		}

		if (tileIsValid && tileAABB2.isInside(*reinterpret_cast<flat::Vector2*>(&newPosition)))
		{
			normal = flat::Vector3(0.f, 0.f, 1.f);
			FLAT_ASSERT(newPosition.z > -FLT_MAX / 2.f);
			newPosition.z = tileZ;
			collidedTileIndex = tileIndex2;
			return;
		}

		flat::Vector2 tileToEntityDirection;
		const float tileToEntityDistance = flat::geometry::intersection::circleToRectangleDistance(tileAABB2, position2d, radius, &tileToEntityDirection);
		if (tileToEntityDistance > 0.f)
		{
			return;
		}

		if (tileIsValid && tileToEntityDistance < radius - 1.f && tileZ > newPosition.z)
		{
			normal = flat::Vector3(0.f, 0.f, 1.f);
			newPosition.z = tileZ;
			collidedTileIndex = tileIndex2;
		}
		else if (tileToEntityDistance < closestCollisionDistance)
		{
			closestCollisionDistance = tileToEntityDistance;
			normal = flat::normalize(flat::Vector3(tileToEntityDirection, 0.f));
			newPosition = newPosition + normal * -tileToEntityDistance;
			collidedTileIndex = tileIndex2;
			//FLAT_DEBUG_ONLY(m_owner->checkValidPosition(newPosition));
		}
	};

	for (int x = minX; x <= maxX; ++x)
	{
		for (int y = minY; y <= maxY; ++y)
		{
			computeTileCollision(x, y);
		}
	}

	FLAT_DEBUG_ONLY(m_owner->checkValidPosition(newPosition));

	if (position != newPosition)
	{
		const map::TileIndex newTileIndex = map->getTileIndex(newPosition.x, newPosition.y);
		if (map::isValidTile(newTileIndex))
		{
			const float newTileZ = map->getTileZ(newTileIndex);
			newPosition.z = std::max(newPosition.z, newTileZ);
		}

		m_owner->setPositionSweep(newPosition);
		FLAT_ASSERT(flat::length2(normal) > 0.f || !map::isValidTile(collidedTileIndex));
		onCollidedWithMap(collidedTileIndex, normal);
	}

	FLAT_DEBUG_ONLY(m_owner->checkValidPosition(newPosition));
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
