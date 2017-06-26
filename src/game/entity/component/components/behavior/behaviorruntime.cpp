#include "behaviorruntime.h"
#include "behavior.h"
#include "behaviorcomponent.h"
#include "../../../entity.h"
#include "../../../entitytemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace behavior
{

BehaviorRuntime::BehaviorRuntime() :
	m_entity(nullptr)
{
	
}

void BehaviorRuntime::enterState(const char* stateName)
{
	if (!m_thread.isEmpty())
	{
		m_thread.stop();
	}

	FLAT_DEBUG_ONLY(m_currentStateName = stateName;)

	m_endSleepTime = -1.f;

	const Behavior& behavior = getBehavior();
	lua_State* L = behavior.getLuaState();
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		// states table
		behavior.pushStates(L);

		//function
		lua_getfield(L, -1, stateName);
		luaL_checktype(L, -1, LUA_TFUNCTION);

		// set thread function
		m_thread.set(L, -1);

		// states table
		lua_pop(L, 1);

		// entity
		lua::pushEntity(L, m_entity);

		// actually start the thread, with 2 arguments: the states table and the entity
		m_thread.start(2);
	}
}

void BehaviorRuntime::sleep(float time, float duration)
{
	m_endSleepTime = time + duration;
}

void BehaviorRuntime::updateCurrentState(float time)
{
	if (m_thread.isRunning() && time >= m_endSleepTime)
	{
		m_thread.update();
	}
}

void BehaviorRuntime::update(float time)
{
	if (m_thread.isFinished())
	{
		enterState("idle");
	}
	else
	{
		updateCurrentState(time);
	}
}

const Behavior& BehaviorRuntime::getBehavior() const
{
	FLAT_ASSERT(m_entity != nullptr);
	const EntityTemplate& entityTemplate = *m_entity->getEntityTemplate().get();
	const BehaviorComponentTemplate* behaviorComponentTemplate = entityTemplate.getComponentTemplate<BehaviorComponent>();
	return behaviorComponentTemplate->getBehavior();
}

} // behavior
} // component
} // entity
} // game



