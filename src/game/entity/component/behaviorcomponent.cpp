#include "behaviorcomponent.h"
#include "../entity.h"
#include "../entitytemplate.h"

namespace game
{
namespace entity
{
namespace component
{

void BehaviorComponent::init()
{
	m_behaviorRuntime.setEntity(m_owner);
	m_owner->addedToMap.on(this, &BehaviorComponent::addedToMap);
}

void BehaviorComponent::update(float currentTime, float elapsedTime)
{
	const int maxTicks = 2;
	for (int numTicks = 0; !m_owner->isBusy() && numTicks < maxTicks; ++numTicks)
	{
		m_behaviorRuntime.update();
	}
	FLAT_DEBUG_ONLY(
		const char* entityName = m_owner->getEntityTemplate()->getName().c_str();
		const char* stateName = m_behaviorRuntime.getCurrentStateName().c_str();
	)
	FLAT_ASSERT_MSG(m_owner->isBusy(), "Behavior warning: %s still idling after %d behavior ticks in state %s", entityName, maxTicks, stateName);
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



