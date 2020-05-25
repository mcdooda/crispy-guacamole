#include "selectioncomponenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace selection
{

void SelectionComponentTemplate::load(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_getfield(L, -1, "canBeSelected");
	m_canBeSelected = lua_toboolean(L, -1) == 1;

	lua_pop(L, 1);
}

} // selection
} // component
} // entity
} // game


