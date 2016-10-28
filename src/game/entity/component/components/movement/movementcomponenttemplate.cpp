#include "movementcomponenttemplate.h"
#include "../../../../game.h"

namespace game
{
namespace entity
{
namespace component
{
namespace movement
{

void MovementComponentTemplate::load(Game& game, lua_State* L, const std::string& entityTemplatePath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_getfield(L, -1, "speed");
	m_speed = static_cast<float>(luaL_checknumber(L, -1));

	lua_getfield(L, -2, "jumpForce");
	m_jumpForce = static_cast<float>(luaL_checknumber(L, -1));

	lua_getfield(L, -3, "weight");
	m_weight = static_cast<float>(luaL_checknumber(L, -1));

	// compute jump height and distance from jump force and weight
	//const float a = -m_weight / 2.f;
	//const float b = m_jumpForce;
	//const float delta = b * b;
	//m_jumpDuration = (-b - std::sqrt(delta)) / (2.f * a);
	m_jumpDuration = (2.f * m_jumpForce) / m_weight;
	m_jumpMaxHeight = getJumpHeight(m_jumpDuration / 2.f);
	m_jumpDistance = m_speed * m_jumpDuration;

	lua_pop(L, 3);
}

} // movement
} // component
} // entity
} // game


