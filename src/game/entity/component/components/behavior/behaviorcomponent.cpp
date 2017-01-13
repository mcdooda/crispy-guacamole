#include "behaviorcomponent.h"
#include "behaviorcomponenttemplate.h"
#include "behaviorevent.h"
#include "../../componenttype.h"
#include "../../../entity.h"
#include "../../../entitytemplate.h"
#include "../detection/detectioncomponent.h"

namespace game
{
namespace entity
{
namespace component
{
namespace behavior
{

void BehaviorComponent::init()
{
	m_behaviorRuntime.setEntity(m_owner);
	m_owner->addedToMap.on(this, &BehaviorComponent::addedToMap);

	if (detection::DetectionComponent* detectionComponent = m_owner->getComponent<detection::DetectionComponent>())
	{
		if (m_behaviorRuntime.isEventHandled<EntityEnteredVisionRangeBehaviorEvent>())
		{
			detectionComponent->entityEnteredVisionRange.on(this, &BehaviorComponent::entityEnteredVisionRange);
		}
		if (m_behaviorRuntime.isEventHandled<EntityLeftVisionRangeBehaviorEvent>())
		{
			detectionComponent->entityLeftVisionRange.on(this, &BehaviorComponent::entityLeftVisionRange);
		}
	}
}

void BehaviorComponent::update(float currentTime, float elapsedTime)
{
	const int maxTicks = 2;
	for (int numTicks = 0; !m_owner->isBusy() && numTicks < maxTicks; ++numTicks)
	{
		m_behaviorRuntime.update();
	}
	/*
	FLAT_DEBUG_ONLY(
		const char* entityName = m_owner->getEntityTemplate()->getName().c_str();
		const char* stateName = m_behaviorRuntime.getCurrentStateName().c_str();
	)
	FLAT_ASSERT_MSG(m_owner->isBusy(), "Behavior warning: %s still idling after %d behavior ticks in state %s", entityName, maxTicks, stateName);
	*/
}

void BehaviorComponent::enterState(const char* stateName)
{
	m_behaviorRuntime.enterState(stateName);
}

void BehaviorComponent::addedToMap(map::Map* map)
{
	enterState("init");
}

void BehaviorComponent::entityEnteredVisionRange(Entity* entity)
{
	m_behaviorRuntime.handleEvent<EntityEnteredVisionRangeBehaviorEvent>(entity);
}

void BehaviorComponent::entityLeftVisionRange(Entity* entity)
{
	m_behaviorRuntime.handleEvent<EntityLeftVisionRangeBehaviorEvent>(entity);
}

#ifdef FLAT_DEBUG
void BehaviorComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	debugDisplay.addText(m_owner->getPosition(), m_behaviorRuntime.getCurrentStateName(), flat::video::Color::BLACK);
}
#endif

} // behavior
} // component
} // entity
} // game



