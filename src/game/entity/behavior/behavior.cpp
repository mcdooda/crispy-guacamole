#include "behavior.h"

namespace game
{
namespace entity
{
namespace behavior
{

Behavior::Behavior(lua_State* L)
{
	load(L);
}

Behavior::~Behavior()
{

}

void Behavior::load(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	m_states.set(L, -1);
}

void Behavior::pushStates(lua_State* L) const
{
	m_states.push(L);
}

} // behavior
} // entity
} // game



