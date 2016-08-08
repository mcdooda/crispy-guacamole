#include "behaviorruntime.h"
#include "behavior.h"
#include "../entity.h"
#include "../entitytemplate.h"

namespace game
{
namespace entity
{
namespace behavior
{

BehaviorRuntime::BehaviorRuntime(Entity* entity) :
	m_entity(entity),
	m_coroutineRef(LUA_NOREF)
{
	FLAT_ASSERT(getBehavior());
}

BehaviorRuntime::~BehaviorRuntime()
{
	if (m_coroutineRef != LUA_NOREF)
	{
		const Behavior* behavior = getBehavior();
		lua_State* L = behavior->pushStates();
		luaL_unref(L, LUA_REGISTRYINDEX, m_coroutineRef);
		m_coroutineRef = LUA_NOREF;
	}
}

void BehaviorRuntime::enterState(const char* stateName)
{
	const Behavior* behavior = getBehavior();
	FLAT_ASSERT(behavior && m_coroutineRef == LUA_NOREF);
	
	lua_State* L = behavior->pushStates();
	
	FLAT_DEBUG_ONLY(int top = lua_gettop(L);)
	
	lua_getfield(L, -1, stateName);
	luaL_checktype(L, -1, LUA_TFUNCTION);
	flat::lua::coroutine::create(L);
	m_coroutineRef = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_pop(L, 1);
	
	FLAT_LUA_ASSERT_MSG(lua_gettop(L) == top - 1, L, "lua_gettop(L) = %d, should be %d", lua_gettop(L), top - 1);
}

void BehaviorRuntime::updateCurrentState()
{
	const Behavior* behavior = getBehavior();
	FLAT_ASSERT(behavior && m_coroutineRef != LUA_NOREF);
	
	lua_State* L = behavior->pushStates();
	
	FLAT_DEBUG_ONLY(int top = lua_gettop(L) - 1;)
	
	lua_rawgeti(L, LUA_REGISTRYINDEX, m_coroutineRef);
	
	lua_pushnil(L); // empty space for coroutine.resume
	lua_pushvalue(L, -2);
	
	// coroutine.resume arguments
	// 1st argument: the states table
	lua_pushvalue(L, -4);
	// 2nd argument: the entity
	lua_pushlightuserdata(L, m_entity);
	
	flat::lua::coroutine::resume(L, 2, 0);
	
	// if the coroutine is dead, leave the current state
	if (flat::lua::coroutine::status(L, -2) == flat::lua::coroutine::CoroutineStatus::DEAD)
	{
		luaL_unref(L, LUA_REGISTRYINDEX, m_coroutineRef);
		m_coroutineRef = LUA_NOREF;
	}
	
	lua_pop(L, 3);
	
	FLAT_LUA_ASSERT_MSG(lua_gettop(L) == top, L, "lua_gettop(L) = %d, should be %d", lua_gettop(L), top);
}

void BehaviorRuntime::update()
{
	if (m_coroutineRef == LUA_NOREF)
	{
		enterState("idle");
	}
	updateCurrentState();
}

const Behavior* BehaviorRuntime::getBehavior() const
{
	FLAT_ASSERT(m_entity);
	std::shared_ptr<const EntityTemplate> entityTemplate = m_entity->getEntityTemplate();
	const EntityTemplate* entityTemplatePtr = entityTemplate.get();
	FLAT_ASSERT(entityTemplatePtr);
	return entityTemplatePtr->getBehavior();
}

} // behavior
} // entity
} // game



