#include "entityhelper.h"
#include "entity.h"
#include "component/components/collision/collisioncomponent.h"
#include "component/components/prop/propcomponent.h"
#include "component/components/movement/movementcomponent.h"

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

float EntityHelper::getDistanceBetweenEntitiesWithRadius(const Entity* a, const Entity* b)
{
	const std::shared_ptr<const EntityTemplate>& aTemplate = a->getEntityTemplate();
	const component::prop::PropComponentTemplate* aPropComponentTemplate = aTemplate->getComponentTemplate<component::prop::PropComponent>();

	const std::shared_ptr<const EntityTemplate>& bTemplate = b->getEntityTemplate();
	const component::prop::PropComponentTemplate* bPropComponentTemplate = bTemplate->getComponentTemplate<component::prop::PropComponent>();

	float distance = flat::length(a->getPosition() - b->getPosition());

	if (aPropComponentTemplate && bPropComponentTemplate)
	{
		distance -= 1.f;
	}
	else if (aPropComponentTemplate)
	{
		if (std::round(a->getPosition().x) == std::round(b->getPosition().x)
		 && std::round(a->getPosition().y) == std::round(b->getPosition().y))
		{
			distance -= 0.5f + getRadius(b);
		}
		else
		{
			distance -= flat::SQRT2 / 2.f + getRadius(b);
		}
	}
	else if (bPropComponentTemplate)
	{
		if (std::round(a->getPosition().x) == std::round(b->getPosition().x)
			&& std::round(a->getPosition().y) == std::round(b->getPosition().y))
		{
			distance -= getRadius(a) + 0.5f;
		}
		else
		{
			distance -= getRadius(a) + flat::SQRT2 / 2.f;
		}
	}
	else
	{
		distance -= getRadius(a) + getRadius(b);
	}

	return distance;
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
	return map::Navigability::ALL;
}

} // entity
} // game


