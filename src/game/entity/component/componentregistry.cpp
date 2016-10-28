#include "componentregistry.h"
#include "components/behavior/behaviorcomponent.h"
#include "components/movement/movementcomponent.h"
#include "components/collision/collisioncomponent.h"
#include "components/sprite/spritecomponent.h"

namespace game
{
namespace entity
{
namespace component
{

ComponentRegistry::ComponentRegistry()
{
	registerComponentTypes();
}

ComponentRegistry::~ComponentRegistry()
{

}

void ComponentRegistry::registerComponentTypes()
{
	// components are updated in the order they are declared
	registerComponentType<behavior::BehaviorComponent>();
	registerComponentType<MovementComponent>();
	registerComponentType<CollisionComponent>();
	registerComponentType<SpriteComponent>();
}

} // component
} // entity
} // game


