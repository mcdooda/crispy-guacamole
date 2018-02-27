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
	flat::Vector2 size = flat::lua::getVector2(L, -1);
	m_width = static_cast<int>(size.x);
	m_height = static_cast<int>(size.y);

	lua_pop(L, 1);
}

} // prop
} // component
} // entity
} // game


