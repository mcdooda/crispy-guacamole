#include "playercontrollercomponenttemplate.h"
#include "../../../../game.h"

namespace game
{
namespace entity
{
namespace component
{
namespace playercontroller
{

void PlayerControllerComponentTemplate::load(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	
	lua_getfield(L, -1, "update");
	m_update.set(L, -1);

	lua_pop(L, 1);
}

} // playercontroller
} // component
} // entity
} // game


