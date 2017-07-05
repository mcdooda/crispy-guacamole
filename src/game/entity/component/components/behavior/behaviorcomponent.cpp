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

	detection::DetectionComponent* detectionComponent = m_owner->getComponent<detection::DetectionComponent>();
	if (detectionComponent != nullptr)
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

void BehaviorComponent::deinit()
{
	//m_behaviorRuntime.setEntity(nullptr); // TODO: uncomment this
	m_owner->addedToMap.off(this);

	if (detection::DetectionComponent* detectionComponent = m_owner->getComponent<detection::DetectionComponent>())
	{
		if (m_behaviorRuntime.isEventHandled<EntityEnteredVisionRangeBehaviorEvent>())
		{
			detectionComponent->entityEnteredVisionRange.off(this);
		}
		if (m_behaviorRuntime.isEventHandled<EntityLeftVisionRangeBehaviorEvent>())
		{
			detectionComponent->entityLeftVisionRange.off(this);
		}
	}
}

void BehaviorComponent::update(float time, float dt)
{
	if (!m_owner->isBusy())
	{
		m_behaviorRuntime.update(time);
	}

	/*
	const int maxTicks = 2;
	for (int numTicks = 0; !m_owner->isBusy() && numTicks < maxTicks; ++numTicks)
	{
		m_behaviorRuntime.update(time);
	}
	
#ifdef FLAT_DEBUG
	const char* entityName = m_owner->getEntityTemplate()->getName().c_str();
	const char* stateName = m_behaviorRuntime.getCurrentStateName().c_str();
	std::string file;
	int line;
	getThreadDebugInfo(file, line);
	FLAT_ASSERT_MSG(m_owner->isBusy(), "Behavior warning: %s still idling after %d behavior ticks in state %s\nCurrently in file %s, line %d", entityName, maxTicks, stateName, file.c_str(), line);
#endif
	*/
}

void BehaviorComponent::enterState(const char* stateName)
{
	m_behaviorRuntime.enterState(stateName);
}

void BehaviorComponent::sleep(float time, float duration)
{
	m_behaviorRuntime.sleep(time, duration);
}

bool BehaviorComponent::addedToMap(Entity* entity, map::Map* map)
{
	FLAT_ASSERT(entity == m_owner);
	if (isEnabled())
	{
		enterState("init");
	}
	return true;
}

bool BehaviorComponent::entityEnteredVisionRange(Entity* entity)
{
	m_behaviorRuntime.handleEvent<EntityEnteredVisionRangeBehaviorEvent>(entity);
	return true;
}

bool BehaviorComponent::entityLeftVisionRange(Entity* entity)
{
	m_behaviorRuntime.handleEvent<EntityLeftVisionRangeBehaviorEvent>(entity);
	return true;
}

#ifdef FLAT_DEBUG
void BehaviorComponent::getThreadDebugInfo(std::string& file, int& line) const
{
	file.clear();
	line = 0;

	const flat::lua::UniqueLuaReference<LUA_TTHREAD>& luaThreadRef = m_behaviorRuntime.getThread().getThread();
	if (!luaThreadRef.isEmpty())
	{
		lua_State* L = luaThreadRef.getLuaState();
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			luaThreadRef.push(L);
			lua_State* L1 = lua_tothread(L, -1);

			lua_Debug ar;
			lua_getstack(L1, 1, &ar);
			lua_getinfo(L1, "Sl", &ar);
			line = ar.currentline;
			file = ar.short_src;

			lua_pop(L, 1);
		}
	}
}

void BehaviorComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	int currentLine;
	std::string currentFile;
	getThreadDebugInfo(currentFile, currentLine);

	if (currentLine != 0)
	{
		debugDisplay.add3dText(
			m_owner->getPosition(),
			m_behaviorRuntime.getCurrentStateName() + "\n" + currentFile + ":" + std::to_string(currentLine),
			flat::video::Color::BLUE
		);
	}
	else
	{
		debugDisplay.add3dText(
			m_owner->getPosition(),
			m_behaviorRuntime.getCurrentStateName(),
			flat::video::Color::RED
		);
	}
}
#endif

} // behavior
} // component
} // entity
} // game



