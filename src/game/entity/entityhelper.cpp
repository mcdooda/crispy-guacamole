#include "entityhelper.h"
#include "entity.h"
#include "component/components/collision/collisioncomponent.h"
#include "component/components/prop/propcomponent.h"
#include "component/components/movement/movementcomponent.h"
#include "../map/map.h"

namespace game
{
namespace entity
{

float EntityHelper::getRadius(const Entity* entity)
{
	const std::shared_ptr<const EntityTemplate>& entityTemplate = entity->getEntityTemplate();
	const component::collision::CollisionComponentTemplate* collisionComponentTemplate = entityTemplate->getComponentTemplate<component::collision::CollisionComponent>();
	return collisionComponentTemplate ? collisionComponentTemplate->getRadius() : 0.f;
}

flat::Vector3 EntityHelper::getCenter(const Entity* entity)
{
	const std::shared_ptr<const EntityTemplate>& entityTemplate = entity->getEntityTemplate();
	const component::prop::PropComponentTemplate* propComponentTemplate = entityTemplate->getComponentTemplate<component::prop::PropComponent>();
	if (propComponentTemplate != nullptr)
	{
		return entity->getPosition() - flat::Vector3((static_cast<float>(propComponentTemplate->getWidth()) - 1.f) * 0.5f, (static_cast<float>(propComponentTemplate->getHeight()) - 1.f) * 0.5f, 0.f);
	}
	return entity->getPosition();
}

void EntityHelper::eachEntityTile(const Entity* entity, std::function<void(map::TileIndex)> func)
{
	const map::Map* map = entity->getMap();
	FLAT_ASSERT_MSG(map != nullptr, "Cannot iterate on tiles from an entity that is not in a map");
	const std::shared_ptr<const EntityTemplate>& entityTemplate = entity->getEntityTemplate();
	const component::prop::PropComponentTemplate* propComponentTemplate = entityTemplate->getComponentTemplate<component::prop::PropComponent>();
	if (propComponentTemplate != nullptr)
	{
		const flat::Vector3& position = entity->getPosition();

		const int maxX = static_cast<int>(position.x);
		const int minX = maxX - propComponentTemplate->getWidth() + 1;

		const int maxY = static_cast<int>(position.y);
		const int minY = maxY - propComponentTemplate->getHeight() + 1;

		for (int x = minX; x <= maxX; ++x)
		{
			for (int y = minY; y <= maxY; ++y)
			{
				const map::TileIndex tileIndex = map->getTileIndex(x, y);
				func(tileIndex);
			}
		}

		return;
	}
	func(entity->getTileIndexFromPosition());
}


bool EntityHelper::canCollide(const Entity* a, const Entity* b)
{
	if (a == b)
	{
		return false;
	}

	const component::collision::CollisionComponentTemplate* aCollisionComponentTemplate = a->getComponentTemplate<component::collision::CollisionComponent>();
	if (aCollisionComponentTemplate == nullptr
		|| !aCollisionComponentTemplate->shouldSeparateFromOtherEntities())
	{
		return false;
	}

	const component::collision::CollisionComponentTemplate* bCollisionComponentTemplate = b->getComponentTemplate<component::collision::CollisionComponent>();
	if (bCollisionComponentTemplate == nullptr || !bCollisionComponentTemplate->shouldSeparateFromOtherEntities())
	{
		return false;
	}

	if (aCollisionComponentTemplate == bCollisionComponentTemplate)
	{
		return aCollisionComponentTemplate->shouldSeparateFromSameType();
	}

	return true;
}

namespace
{
void locGetPropEntityBounds(const flat::Vector3& position, const component::prop::PropComponentTemplate& propComponentTemplate, flat::AABB2& bounds)
{
	const float maxX = position.x + 0.5f;
	bounds.min.x = maxX - static_cast<float>(propComponentTemplate.getWidth());
	bounds.max.x = maxX;

	const float maxY = position.y + 0.5f;
	bounds.min.y = maxY - static_cast<float>(propComponentTemplate.getHeight());
	bounds.max.y = maxY;
}
}

float EntityHelper::getDistanceBetweenEntitiesWithRadius(const Entity* a, const Entity* b)
{
	const std::shared_ptr<const EntityTemplate>& aTemplate = a->getEntityTemplate();
	const component::prop::PropComponentTemplate* aPropComponentTemplate = aTemplate->getComponentTemplate<component::prop::PropComponent>();

	const std::shared_ptr<const EntityTemplate>& bTemplate = b->getEntityTemplate();
	const component::prop::PropComponentTemplate* bPropComponentTemplate = bTemplate->getComponentTemplate<component::prop::PropComponent>();

	if (aPropComponentTemplate && bPropComponentTemplate)
	{
		flat::AABB2 aBounds;
		locGetPropEntityBounds(a->getPosition(), *aPropComponentTemplate, aBounds);
		flat::AABB2 bBounds;
		locGetPropEntityBounds(b->getPosition(), *bPropComponentTemplate, bBounds);
		return flat::geometry::intersection::rectangleToRectangleDistance(aBounds, bBounds);
	}
	else if (aPropComponentTemplate)
	{
		flat::AABB2 bounds;
		locGetPropEntityBounds(a->getPosition(), *aPropComponentTemplate, bounds);
		return flat::geometry::intersection::circleToRectangleDistance(bounds, flat::Vector2(b->getPosition()), getRadius(b));
	}
	else if (bPropComponentTemplate)
	{
		flat::AABB2 bounds;
		locGetPropEntityBounds(b->getPosition(), *bPropComponentTemplate, bounds);
		return flat::geometry::intersection::circleToRectangleDistance(bounds, flat::Vector2(a->getPosition()), getRadius(a));
	}
	else
	{
		return flat::length(a->getPosition() - b->getPosition()) - getRadius(a) - getRadius(b);
	}
}

map::Navigability EntityHelper::getNavigabilityMask(const Entity* entity)
{
	return getNavigabilityMask(entity->getEntityTemplate().get());
}

map::Navigability EntityHelper::getNavigabilityMask(const EntityTemplate* entityTemplate)
{
	const component::movement::MovementComponentTemplate* movementComponentTemplate = entityTemplate->getComponentTemplate<component::movement::MovementComponent>();
	if (movementComponentTemplate != nullptr)
	{
		return movementComponentTemplate->getNavigabilityMask();
	}
	const component::collision::CollisionComponentTemplate* collisionComponentTemplate = entityTemplate->getComponentTemplate<component::collision::CollisionComponent>();
	if (collisionComponentTemplate != nullptr)
	{
		if (!collisionComponentTemplate->shouldSeparateFromTiles())
		{
			return map::Navigability::NONE;
		}
	}
	return map::Navigability::ALL;
}

} // entity
} // game


