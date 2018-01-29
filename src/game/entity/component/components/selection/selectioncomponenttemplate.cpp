#include "selectioncomponenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace selection
{

void SelectionComponentTemplate::load(Game& game, lua_State* L, const std::string& entityTemplatePath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
}

} // selection
} // component
} // entity
} // game


