#include "entity/component/lua/componentregistry.h"
#include "entity/component/componentregistry.h"
#include "states/basemapstate.h"
#include "game.h"

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
		{"allExcept",     l_Components_allExcept},
		{"getVisualName", l_Components_getVisualName},

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
	const int top = lua_gettop(L);
	for (int i = 1; i <= top; ++i)
	{
		const ComponentFlags flag = static_cast<ComponentFlags>(luaL_checkinteger(L, i));
		componentFlags &= ~flag;
	}
	lua_pushinteger(L, componentFlags);
	return 1;
}

int l_Components_getVisualName(lua_State* L)
{
	const ComponentFlags componentFlag = static_cast<ComponentFlags>(luaL_checkinteger(L, 1));
	const Game& game = flat::lua::getFlatAs<Game>(L);
	const states::BaseMapState& baseMapState = game.getStateMachine().getState()->to<states::BaseMapState>();
	const ComponentRegistry& componentRegistry = baseMapState.getComponentRegistry();
	const ComponentType& componentType = componentRegistry.getComponentType(componentFlag);
	lua_pushstring(L, componentType.getVisualName());
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


