#include "behaviorcomponent.h"
#include "../entity.h"

namespace game
{
namespace entity
{
namespace component
{

void BehaviorComponent::setOwner(Entity* owner)
{
	Super::setOwner(owner);
	m_behaviorRuntime.setEntity(owner);
	
	owner->addedToMap.on(this, &BehaviorComponent::addedToMap);
}

void BehaviorComponent::update(float currentTime, float elapsedTime)
{
	if (!m_owner->isBusy())
	{
		m_behaviorRuntime.update();
	}
}

void BehaviorComponent::enterState(const char* stateName)
{
	m_behaviorRuntime.enterState(stateName);
}

void BehaviorComponent::addedToMap(map::Map* map)
{
	enterState("init");
}

} // component
} // entity
} // game



