#include "componentregistry.h"
#include "../../../game.h"
#include "../../../states/basemapstate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace lua
{

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	static const luaL_Reg ComponentRegistry_lib_s[] = {
		{"getConfigNames", l_ComponentRegistry_getConfigNames},

		{nullptr, nullptr}
	};

	luaL_newlib(L, ComponentRegistry_lib_s);
	lua_setglobal(L, "ComponentRegistry");

	return 0;
}

int l_ComponentRegistry_getConfigNames(lua_State* L)
{
	Game& game = flat::lua::getGameAs<Game>(L);
	states::BaseMapState& baseMapState = *static_cast<states::BaseMapState*>(game.getStateMachine().getState());
	const entity::component::ComponentRegistry& componentRegistry = baseMapState.getComponentRegistry();
	int numComponentTypes = static_cast<int>(componentRegistry.getNumComponentTypes());
	lua_createtable(L, numComponentTypes, 0);
	int i = 1;
	componentRegistry.eachComponentType([L, &i](const ComponentType& componentType)
	{
		lua_pushstring(L, componentType.getConfigName());
		lua_rawseti(L, -2, i);
		++i;
	});
	return 1;
}

} // lua
} // component
} // entity
} // game


