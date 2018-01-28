#include <flat.h>
#include "tilescontainer.h"
#include "../brush.h"

namespace game
{
namespace map
{
namespace brush
{
namespace lua
{

using LuaTilesContainer = flat::lua::SharedCppReference<TilesContainer>;

int openTilesContainer(flat::Flat& flat)
{
	lua_State* L = flat.lua->getMainState();
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	// TilesContainer metatable
	static const luaL_Reg TilesContainer_lib_m[] = {
		{ "eachTile", l_TilesContainer_eachTile },

		{ nullptr, nullptr }
	};
	flat.lua->registerClass<LuaTilesContainer>("CG.TilesContainer", TilesContainer_lib_m);

	return 0;
}

int l_TilesContainer_eachTile(lua_State* L)
{
	TilesContainer& tilesContainer = getTilesContainer(L, 1);
	luaL_checktype(L, 2, LUA_TFUNCTION);
	for (TileEffect& tileEffect : tilesContainer)
	{
		lua_pushvalue(L, 2);
		lua_pushlightuserdata(L, tileEffect.tile);
		lua_pushnumber(L, tileEffect.effect);
		lua_call(L, 2, 0);
	}
	return 0;
}

// private
TilesContainer& getTilesContainer(lua_State* L, int index)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	return LuaTilesContainer::get(L, index);
}

void pushTilesContainer(lua_State* L, TilesContainer* tilesContainer)
{
	LuaTilesContainer::pushNew(L, tilesContainer);
}

} // lua
} // brush
} // map
} // game


