#include "entityhelper.h"
#include "entity.h"
#include "component/components/collision/collisioncomponent.h"

namespace game
{
namespace entity
{

float EntityHelper::getRadius(const Entity* entity)
{
	const std::shared_ptr<const EntityTemplate>& targetEntityTemplate = entity->getEntityTemplate();
	const component::collision::CollisionComponentTemplate* targetCollisionComponentTemplate = targetEntityTemplate->getComponentTemplate<component::collision::CollisionComponent>();
	return targetCollisionComponentTemplate ? targetCollisionComponentTemplate->getRadius() : 0.f;
}

} // entity
} // game


