#include "projectilecomponenttemplate.h"
#include "../../../../game.h"

namespace game
{
namespace entity
{
namespace component
{
namespace projectile
{

void ProjectileComponentTemplate::load(Game& game, lua_State* L, const std::string& entityTemplatePath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_getfield(L, -1, "weight");
	m_weight = static_cast<float>(luaL_checknumber(L, -1));

	lua_getfield(L, -2, "speed");
	m_speed = static_cast<float>(luaL_checknumber(L, -1));

	lua_getfield(L, -3, "collidedCallback");
	m_collidedCallback.set(L, -1);

	lua_pop(L, 3);
}

} // projectile
} // component
} // entity
} // game


