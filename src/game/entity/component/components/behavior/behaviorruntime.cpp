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
	m_entity(nullptr),
	m_hasIdle(false)
{

}

void BehaviorRuntime::setEntity(Entity* entity)
{
	m_entity = entity;
	m_hasIdle = hasState("idle");
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
		if (lua_isnil(L, -1))
		{
			luaL_error(L, "'%s' has no '%s' state", m_entity->getTemplateName().c_str(), stateName);
		}

		luaL_checktype(L, -1, LUA_TFUNCTION);

		// set thread function
		m_thread.set(L, -1);

		lua_pop(L, 2);
	}

	startThread();
}

bool BehaviorRuntime::hasState(const char* stateName)
{
	const Behavior& behavior = getBehavior();
	lua_State* L = behavior.getLuaState();
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		// states table
		behavior.pushStates(L);

		//function
		lua_getfield(L, -1, stateName);
		const bool hasState = lua_isfunction(L, -1) == 1;

		lua_pop(L, 2);

		return hasState;
	}
}

void BehaviorRuntime::sleep(float time, float duration)
{
	m_endSleepTime = time + duration;
}

void BehaviorRuntime::updateCurrentState(float time)
{
	if (time < m_endSleepTime)
	{
		return;
	}

	if (m_thread.isRunning())
	{
		int status = m_thread.update(1);

		if (status == LUA_OK)
		{
			const Behavior& behavior = getBehavior();
			lua_State* L = behavior.getLuaState();
			{
				m_thread.reset();
				if (lua_type(L, -1) == LUA_TSTRING)
				{
					behavior.pushStates(L);

					// state name
					lua_pushvalue(L, -2);

					// get function
					lua_rawget(L, -2);

					m_thread.set(L, -1);
				}
				else if (!lua_isnil(L, -1))
				{
					luaL_error(L, "Behavior thread returned invalid value '%s'", lua_tostring(L, -1));
				}
			}
		}
	}
}

void BehaviorRuntime::update(float time)
{
	if (m_thread.isFinished())
	{
		if (!m_thread.isEmpty())
		{
			startThread();
		}
		else if (m_hasIdle)
		{
			enterState("idle");
		}
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

void BehaviorRuntime::startThread()
{
	const Behavior& behavior = getBehavior();
	lua_State* L = behavior.getLuaState();
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		// states table
		behavior.pushStates(L);

		// entity
		lua::pushEntity(L, m_entity);

		// actually start the thread, with 2 arguments: the states table and the entity
		m_thread.start(2);
	}
}

} // behavior
} // component
} // entity
} // game



