#include "collisioncomponenttemplate.h"
#include "../../../../game.h"

namespace game
{
namespace entity
{
namespace component
{

void CollisionComponentTemplate::load(Game& game, lua_State* L, const std::string& entityTemplatePath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_getfield(L, -1, "radius");
	m_radius = static_cast<float>(luaL_checknumber(L, -1));

	lua_pop(L, 1);
}

} // component
} // entity
} // game


