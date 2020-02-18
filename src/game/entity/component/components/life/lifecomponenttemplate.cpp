#include "lifecomponenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace life
{

void LifeComponentTemplate::load(Game& game, lua_State* L, const std::string& entityTemplatePath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	
	lua_getfield(L, -1, "spawn");
	if (!lua_isnil(L, -1))
	{
		m_spawnFunc.set(L, -1);
	}
	
	lua_getfield(L, -2, "despawn");
	if (!lua_isnil(L, -1))
	{
		m_despawnFunc.set(L, -1);
	}

	lua_getfield(L, -3, "damageTaken");
	if (!lua_isnil(L, -1))
	{
		m_damageTakenFunc.set(L, -1);
	}
	
	lua_getfield(L, -4, "maxHealth");
	m_maxHealth = static_cast<int>(luaL_checkinteger(L, -1));
	
	lua_pop(L, 4);
}

} // life
} // component
} // entity
} // game


