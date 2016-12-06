#include "entitythread.h"
#include "lua/entity.h"

namespace game
{
namespace entity
{

EntityThread::EntityThread() :
	m_mainThread(nullptr),
	m_status(LUA_OK)
{
	
}

EntityThread::~EntityThread()
{

}

void EntityThread::set(lua_State* L, int index)
{
	m_mainThread = L;
	m_function.set(L, index);
}

void EntityThread::start(Entity* entity)
{
	FLAT_ASSERT(m_thread.isEmpty());

	lua_State* L = m_mainThread;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		lua_State* L1 = lua_newthread(L);
		m_thread.set(L, -1);

		m_function.push(L);
		lua::pushEntity(L, entity);
		lua_xmove(L, L1, 2);

		m_status = lua_resume(L1, nullptr, 1);
		if (m_status == LUA_OK)
		{
			m_thread.reset();
		}
		else if (m_status != LUA_YIELD)
		{
			lua_error(L1);
		}

		lua_pop(L, 1);
	}
}

void EntityThread::update(Entity* entity)
{
	FLAT_ASSERT(isRunning());

	lua_State* L = m_mainThread;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		m_thread.push(L);
		lua_State* L1 = lua_tothread(L, -1);
		FLAT_ASSERT(L1 != nullptr);

		m_status = lua_resume(L1, nullptr, 0);
		if (m_status == LUA_OK)
		{
			m_thread.reset();
		}
		else if (m_status != LUA_YIELD)
		{
			lua_error(L1);
		}

		lua_pop(L, 1);
	}
}

} // entity
} // game


