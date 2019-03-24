#include "collisioncomponenttemplate.h"
#include "../../../../game.h"

namespace game
{
namespace entity
{
namespace component
{
namespace collision
{

void CollisionComponentTemplate::load(Game& game, lua_State* L, const std::string& entityTemplatePath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_getfield(L, -1, "type");
	luaL_checktype(L, -1, LUA_TSTRING);

	lua_pushstring(L, "cylinder");
	lua_pushstring(L, "sphere");

	const int typeIndex = -3;
	const int cylinderTypeIndex = -2;
	const int sphereTypeIndex = -1;

	if (lua_compare(L, typeIndex, cylinderTypeIndex, LUA_OPEQ))
	{
		m_collisionBox.type = CollisionType::CYLINDER;
	}
	else if (lua_compare(L, typeIndex, sphereTypeIndex, LUA_OPEQ))
	{
		m_collisionBox.type = CollisionType::SPHERE;
	}
	else
	{
		luaL_error(L, "Unknown box type: %s", lua_tostring(L, typeIndex));
	}
	lua_pop(L, -typeIndex);

	switch (m_collisionBox.type)
	{
		case CollisionType::CYLINDER:
		{
			lua_getfield(L, -1, "radius");
			m_collisionBox.cylinder.radius = static_cast<float>(luaL_checknumber(L, -1));

			lua_getfield(L, -2, "height");
			m_collisionBox.cylinder.height = static_cast<float>(luaL_checknumber(L, -1));

			lua_pop(L, 2);
			break;
		}
		case CollisionType::SPHERE:
		{
			lua_getfield(L, -1, "radius");
			m_collisionBox.sphere.radius = static_cast<float>(luaL_checknumber(L, -1));

			lua_pop(L, 1);
			break;
		}
	}

	lua_getfield(L, -1, "separate");
	luaL_checktype(L, -1, LUA_TBOOLEAN);
	m_separate = lua_toboolean(L, -1) == 1;

	lua_pop(L, 1);

	lua_getfield(L, -1, "separateSameType");
	if (lua_isnil(L, -1))
	{
		m_separateSameType = m_separate;
	}
	else
	{
		luaL_checktype(L, -1, LUA_TBOOLEAN);
		m_separateSameType = lua_toboolean(L, -1) == 1;
	}

	lua_pop(L, 1);
}

} // collision
} // component
} // entity
} // game


