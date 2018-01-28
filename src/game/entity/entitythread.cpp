#include "entitythread.h"
#include "lua/entity.h"

namespace game
{
namespace entity
{

void EntityThread::start(lua_State* L, Entity* entity)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	lua::pushEntity(L, entity);
	Super::start(L, 1);
}

} // entity
} // game


