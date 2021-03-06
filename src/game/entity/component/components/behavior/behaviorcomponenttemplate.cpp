#include "behaviorcomponenttemplate.h"
#include "../../../../game.h"

namespace game
{
namespace entity
{
namespace component
{
namespace behavior
{

void BehaviorComponentTemplate::load(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	m_behavior = std::make_unique<behavior::Behavior>(L);
}

} // behavior
} // component
} // entity
} // game


