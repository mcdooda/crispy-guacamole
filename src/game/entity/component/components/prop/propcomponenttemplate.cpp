#include "propcomponenttemplate.h"
#include "../../../../game.h"

namespace game
{
namespace entity
{
namespace component
{
namespace prop
{

void PropComponentTemplate::load(Game& game, lua_State* L, const std::string& entityTemplatePath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	// prop size
	lua_getfield(L, -1, "size");
	m_size = flat::lua::getVector2(L, -1);

	// navigability
	lua_getfield(L, -2, "navigability");
	m_navigability = static_cast<map::Navigability>(luaL_checkinteger(L, -1));

	lua_pop(L, 2);
}

} // prop
} // component
} // entity
} // game


