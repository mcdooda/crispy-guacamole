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

}

} // prop
} // component
} // entity
} // game


