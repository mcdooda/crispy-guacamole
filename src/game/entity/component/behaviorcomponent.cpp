#include "behaviorcomponent.h"
#include "../entity.h"

namespace game
{
namespace entity
{
namespace component
{

void BehaviorComponent::setOwner(Entity* entity)
{
	Super::setOwner(entity);
	m_behaviorRuntime.setEntity(entity);
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

} // component
} // entity
} // game



