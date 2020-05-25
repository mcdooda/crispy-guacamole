#include "attackcomponenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace attack
{

void AttackComponentTemplate::load(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_getfield(L, -1, "attack");
	m_attack.set(L, -1);

	lua_getfield(L, -2, "attackRange");
	m_attackRange = static_cast<float>(luaL_checknumber(L, -1));

	lua_getfield(L, -3, "attackCooldown");
	m_attackCooldown = static_cast<float>(luaL_checknumber(L, -1));

	lua_getfield(L, -4, "autoAttack");
	m_autoAttack = lua_toboolean(L, -1) == 1;

	lua_getfield(L, -5, "moveDuringAttack");
	m_moveDuringAttack = lua_toboolean(L, -1) == 1;

	lua_getfield(L, -6, "attackDuringMove");
	m_attackDuringMove = lua_toboolean(L, -1) == 1;

	lua_pop(L, 6);
}

} // attack
} // component
} // entity
} // game


