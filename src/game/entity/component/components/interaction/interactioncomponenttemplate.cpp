#include "interactioncomponenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace interaction
{

void InteractionComponentTemplate::load(Game& game, lua_State* L, const std::string& entityTemplatePath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	
	lua_getfield(L, -1, "behaviorStateName");
	m_behaviorStateName = luaL_checkstring(L, -1);
	
	lua_pop(L, 1);
}

} // interaction
} // component
} // entity
} // game


