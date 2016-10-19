#include "componentregistry.h"
#include "components/behavior/behaviorcomponent.h"
#include "components/movement/movementcomponent.h"
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
	registerComponentType<BehaviorComponent>();
	registerComponentType<MovementComponent>();
	registerComponentType<SpriteComponent>();
}

} // component
} // entity
} // game


