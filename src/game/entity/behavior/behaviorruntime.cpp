#include "behaviorruntime.h"
#include "behavior.h"
#include "../entity.h"
#include "../entitytemplate.h"
#include "../lua/entity.h"

namespace game
{
namespace entity
{
namespace behavior
{

BehaviorRuntime::BehaviorRuntime() :
	m_entity(nullptr),
	m_coroutineRef(LUA_NOREF)
{
	
}

BehaviorRuntime::~BehaviorRuntime()
{
	if (m_coroutineRef != LUA_NOREF)
	{
		const Behavior* behavior = getBehavior();
		lua_State* L = behavior->getLuaState();
		luaL_unref(L, LUA_REGISTRYINDEX, m_coroutineRef);
		m_coroutineRef = LUA_NOREF;
	}
}

void BehaviorRuntime::enterState(const char* stateName)
{
	const Behavior* behavior = getBehavior();
	
	lua_State* L = behavior->getLuaState();
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	
	// new thread
	luaL_unref(L, LUA_REGISTRYINDEX, m_coroutineRef);
	lua_State* L1 = lua_newthread(L);
	m_coroutineRef = luaL_ref(L, LUA_REGISTRYINDEX);
	
	// states table
	behavior->pushStates(L);
	
	//function
	lua_getfield(L, -1, stateName);
	luaL_checktype(L, -1, LUA_TFUNCTION);
	
	// states table
	lua_pushvalue(L, -2);
	
	// entity
	lua::pushEntity(L, m_entity);
	
	// move the function and arguments to the new thread
	lua_xmove(L, L1, 3);
	int status = lua_resume(L1, nullptr, 2);
	if (status == LUA_OK)
	{
		luaL_unref(L, LUA_REGISTRYINDEX, m_coroutineRef);
		m_coroutineRef = LUA_NOREF;
	}
	else if (status != LUA_YIELD)
	{
		lua_error(L1);
	}
	
	lua_pop(L, 1);
}

void BehaviorRuntime::updateCurrentState()
{
	const Behavior* behavior = getBehavior();
	FLAT_ASSERT(m_coroutineRef != LUA_NOREF);
	
	lua_State* L = behavior->getLuaState();
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	
	lua_rawgeti(L, LUA_REGISTRYINDEX, m_coroutineRef);
	luaL_checktype(L, -1, LUA_TTHREAD);
	lua_State* L1 = lua_tothread(L, -1);
	FLAT_ASSERT(L1 != nullptr);
	
	int status = lua_resume(L1, nullptr, 0);
	if (status == LUA_OK)
	{
		luaL_unref(L, LUA_REGISTRYINDEX, m_coroutineRef);
		m_coroutineRef = LUA_NOREF;
	}
	else if (status != LUA_YIELD)
	{
		lua_error(L1);
	}
	
	lua_pop(L, 1);
}

void BehaviorRuntime::update()
{
	if (m_coroutineRef == LUA_NOREF)
	{
		enterState("idle");
	}
	else
	{
		updateCurrentState();
	}
}

const Behavior* BehaviorRuntime::getBehavior() const
{
	FLAT_ASSERT(m_entity != nullptr);
	const std::shared_ptr<const EntityTemplate>& entityTemplate = m_entity->getEntityTemplate();
	const EntityTemplate* entityTemplatePtr = entityTemplate.get();
	FLAT_ASSERT(entityTemplatePtr != nullptr);
	const Behavior* behavior = entityTemplatePtr->getBehavior();
	FLAT_ASSERT(behavior != nullptr);
	return behavior;
}

} // behavior
} // entity
} // game



