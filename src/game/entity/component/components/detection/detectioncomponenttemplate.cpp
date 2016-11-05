#include "detectioncomponenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace detection
{

void DetectionComponentTemplate::load(Game& game, lua_State* L, const std::string& entityTemplatePath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_getfield(L, -1, "visionRange");
	m_visionRange = static_cast<float>(luaL_checknumber(L, -1));

	lua_pop(L, 1);
}

} // detection
} // component
} // entity
} // game


