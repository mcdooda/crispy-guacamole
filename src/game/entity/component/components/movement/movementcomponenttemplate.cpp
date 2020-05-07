#include "movementcomponenttemplate.h"

#include "game.h"

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

	lua_getfield(L, -1, "walkedOnTile");
	m_walkedOnTile.setIfNotNil(L, -1);

	lua_getfield(L, -2, "speed");
	m_speed = static_cast<float>(luaL_checknumber(L, -1));

	lua_getfield(L, -3, "jumpForce");
	m_jumpForce = static_cast<float>(luaL_checknumber(L, -1));

	lua_getfield(L, -4, "weight");
	m_weight = static_cast<float>(luaL_checknumber(L, -1));

	// compute jump height and distance from jump force and weight
	//const float a = -m_weight / 2.f;
	//const float b = m_jumpForce;
	//const float delta = b * b;
	//m_jumpDuration = (-b - std::sqrt(delta)) / (2.f * a);
	m_jumpDuration = (2.f * m_jumpForce) / m_weight;
	m_jumpMaxHeight = getJumpHeight(m_jumpDuration / 2.f);
	m_jumpDistance = m_speed * m_jumpDuration;

	lua_getfield(L, -5, "snapToGround");
	m_snapToGround = lua_toboolean(L, -1) == 1;

	lua_getfield(L, -6, "navigabilityMask");
	if (!lua_isnil(L, -1))
	{
		m_navigabilityMask = static_cast<map::Navigability>(luaL_checkinteger(L, -1));
	}
	else
	{
		m_navigabilityMask = map::Navigability::GROUND;
	}

	lua_pop(L, 6);
}

} // movement
} // component
} // entity
} // game


