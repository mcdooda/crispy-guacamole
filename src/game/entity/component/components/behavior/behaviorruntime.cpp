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
	m_endSleepTime(-1.f),
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
		FLAT_DEBUG_ONLY(m_currentStateName = stateName;)

		lua_pop(L, 2);
	}

	updateCurrentState();
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

void BehaviorRuntime::updateCurrentState()
{
	const Behavior& behavior = getBehavior();
	lua_State* L = behavior.getLuaState();
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		// states table
		behavior.pushStates(L);

		// entity
		lua::pushEntity(L, m_entity);

		int status = m_thread.resume(2, 1);

		if (status == LUA_OK)
		{
			m_thread.reset();
			if (lua_type(L, -1) == LUA_TSTRING)
			{
				FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

				const char* stateName = lua_tostring(L, -1);

				behavior.pushStates(L);

				// state name
				lua_pushvalue(L, -2);

				// get function
				lua_rawget(L, -2);

				if (lua_isnil(L, -1))
				{
					luaL_error(L, "'%s' has no '%s' state", m_entity->getTemplateName().c_str(), stateName);
				}

				luaL_checktype(L, -1, LUA_TFUNCTION);

				// set thread function
				m_thread.set(L, -1);
				FLAT_DEBUG_ONLY(m_currentStateName = stateName;)

				lua_pop(L, 2);
			}
			else if (lua_isnil(L, -1))
			{
				FLAT_DEBUG_ONLY(m_currentStateName = "";)
			}
			else
			{
				luaL_error(L, "Behavior thread returned invalid value '%s'", lua_tostring(L, -1));
			}
		}

		lua_pop(L, 1);
	}
}

void BehaviorRuntime::update(float time)
{
	if (time < m_endSleepTime)
	{
		return;
	}

	if (m_thread.hasFunction())
	{
		updateCurrentState();
	}
	else if (m_hasIdle)
	{
		enterState("idle");
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



