#include "componentregistry.h"
#include "../componentregistry.h"

namespace game
{
namespace entity
{
namespace component
{
namespace lua
{

int open(lua_State* L, const ComponentRegistry& componentRegistry)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_createtable(L, 0, 1);
	static const luaL_Reg l_Components_lib_f[] = {
		{"allExcept", l_Components_allExcept},

		{nullptr, nullptr}
	};
	luaL_setfuncs(L, l_Components_lib_f, 0);
	lua_setglobal(L, "Components");

	setComponentFlagsTable(L, componentRegistry);

	return 0;
}

int l_Components_allExcept(lua_State* L)
{
	ComponentFlags componentFlags = AllComponents;
	int top = lua_gettop(L);
	for (int i = 1; i <= top; ++i)
	{
		ComponentFlags flag = static_cast<ComponentFlags>(luaL_checkinteger(L, i));
		componentFlags &= ~flag;
	}
	lua_pushinteger(L, componentFlags);
	return 1;
}

int setComponentFlagsTable(lua_State* L, const ComponentRegistry& componentRegistry)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	int numComponentTypes = static_cast<int>(componentRegistry.getNumComponentTypes());
	lua_createtable(L, numComponentTypes, 0);
	componentRegistry.eachComponentType([L](const ComponentType& componentType)
	{
		lua_pushstring(L, componentType.getConfigName());
		lua_pushinteger(L, componentType.getComponentTypeFlag());
		lua_rawset(L, -3);
	});
	lua_setglobal(L, "Component");

	return 0;
}

} // lua
} // component
} // entity
} // game


