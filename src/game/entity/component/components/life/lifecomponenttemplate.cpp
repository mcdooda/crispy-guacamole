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
	m_spawnFunc.set(L, -1);
	
	lua_getfield(L, -2, "despawn");
	m_despawnFunc.set(L, -1);
	
	lua_getfield(L, -3, "maxHealth");
	m_maxHealth = luaL_checkint(L, -1);
	
	lua_pop(L, 3);
}

} // life
} // component
} // entity
} // game


