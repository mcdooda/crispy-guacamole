#ifndef GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIORRUNTIME_H
#define GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIORRUNTIME_H

#include <memory>
#include "behavior.h"
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

class BehaviorRuntime final
{
	public:
		BehaviorRuntime();
		BehaviorRuntime(const BehaviorRuntime&) = delete;
		void operator=(const BehaviorRuntime&) = delete;
		
		void setEntity(lua_State* L, Entity* entity);
		
		void enterState(lua_State* L, const char* stateName);

		void sleep(float time, float duration);

		template <class EventType, class... T>
		void handleEvent(lua_State* L, T... params);

		template <class EventType>
		bool isEventHandled(lua_State* L);

		void updateCurrentState(lua_State* L, float time);
		void update(lua_State* L, float time);

		void reset(lua_State* L);

		FLAT_DEBUG_ONLY(inline const std::string& getCurrentStateName() const { return m_currentStateName; })
		FLAT_DEBUG_ONLY(inline const flat::lua::Thread& getThread() const { return m_thread; })
		
	private:
		const Behavior& getBehavior() const;
		
	private:
		Entity* m_entity;
		flat::lua::Thread m_thread;
		float m_endSleepTime;
		bool m_hasIdle;
		FLAT_DEBUG_ONLY(std::string m_currentStateName;)
};

template <class EventType, class... T>
void BehaviorRuntime::handleEvent(lua_State* L, T... params)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	// states table
	const Behavior& behavior = getBehavior();
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
		enterState(L, stateName);
	}

	lua_pop(L, 2);
}

template<class EventType>
inline bool component::behavior::BehaviorRuntime::isEventHandled(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	// push states table
	const Behavior& behavior = getBehavior();
	behavior.pushStates(L);

	// push function
	lua_getfield(L, -1, EventType::getMethodName());
	bool eventHandled = lua_isfunction(L, -1);

	lua_pop(L, 2);

	return eventHandled;
}

} // behavior
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIORRUNTIME_H


