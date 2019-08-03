#include "entitythread.h"
#include "lua/entity.h"

namespace game
{
namespace entity
{

void EntityThread::start(Entity* entity)
{
	lua_State* L = m_function.getLuaState();
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
		lua::pushEntity(L, entity);
		resume(1, 0);
	}
}

} // entity
} // game


