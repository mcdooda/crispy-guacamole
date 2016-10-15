#include "behaviorcomponenttemplate.h"
#include "../../../../game.h"

namespace game
{
namespace entity
{
namespace component
{

void BehaviorComponentTemplate::load(Game& game, lua_State* L, const std::string& entityTemplatePath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	m_behavior = new behavior::Behavior(L);
}

} // component
} // entity
} // game


