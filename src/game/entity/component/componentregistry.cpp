#include "componentregistry.h"
#include "behaviorcomponent.h"
#include "movementcomponent.h"
#include "spritecomponent.h"

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

void ComponentRegistry::eachComponentType(std::function<void(const ComponentType*)> func)
{
	for (std::shared_ptr<ComponentType>& componentType : m_componentTypes)
	{
		func(componentType.get());
	}
}

} // component
} // entity
} // game


