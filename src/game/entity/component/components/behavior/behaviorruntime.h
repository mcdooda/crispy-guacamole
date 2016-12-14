#ifndef GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIORRUNTIME_H
#define GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIORRUNTIME_H

#include <flat.h>
#include <memory>
#include "../../../lua/entity.h"

namespace game
{
namespace entity
{
class Entity;
namespace component
{
namespace behavior
{
class Behavior;

class BehaviorRuntime final
{
	public:
		BehaviorRuntime();
		BehaviorRuntime(const BehaviorRuntime&) = delete;
		void operator=(const BehaviorRuntime&) = delete;
		~BehaviorRuntime();
		
		inline void setEntity(Entity* entity) { m_entity = entity; }
		
		void enterState(const char* stateName);

		template <class EventType, class... T>
		void handleEvent(T... params);

		template <class EventType>
		bool isEventHandled();

		void updateCurrentState();
		void update();

		FLAT_DEBUG_ONLY(inline const std::string& getCurrentStateName() const { return m_currentStateName; })
		
	private:
		const Behavior& getBehavior() const;
		
	private:
		Entity* m_entity;
		int m_coroutineRef; // TODO: replace by SharedLuaReference
		FLAT_DEBUG_ONLY(std::string m_currentStateName;)
};

template <class EventType, class... T>
void BehaviorRuntime::handleEvent(T... params)
{
	const Behavior& behavior = getBehavior();

	lua_State* L = behavior.getLuaState();
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		// states table
		behavior.pushStates(L);

		//function
		lua_getfield(L, -1, EventType::getMethodName());
		luaL_checktype(L, -1, LUA_TFUNCTION);

		// states table
		lua_pushvalue(L, -2);

		lua::pushEntity(L, m_entity);
		int numParams = EventType::push(L, params...);

		lua_call(L, numParams + 2, 1);

		if (!lua_isnil(L, -1))
		{
			const char* stateName = luaL_checkstring(L, -1);
			enterState(stateName);
		}

		lua_pop(L, 2);
	}
}

template<class EventType>
inline bool component::behavior::BehaviorRuntime::isEventHandled()
{
	bool eventHandled = false;

	const Behavior& behavior = getBehavior();

	lua_State* L = behavior.getLuaState();
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		// states table
		behavior.pushStates(L);

		//function
		lua_getfield(L, -1, EventType::getMethodName());
		eventHandled = lua_isfunction(L, -1);

		lua_pop(L, 2);
	}

	return eventHandled;
}

} // behavior
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIORRUNTIME_H


