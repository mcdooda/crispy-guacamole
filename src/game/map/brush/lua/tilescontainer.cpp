#include <flat.h>
#include "tilescontainer.h"
#include "../brush.h"
#include "lua/table.h"
#include "map/lua/map.h"
#include "map/map.h"

namespace game::map::brush::lua
{

using LuaTilesContainer = flat::lua::SharedCppReference<TilesContainer>;

int openTilesContainer(flat::Flat& flat)
{
	lua_State* L = flat.lua->state;
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	// TilesContainer metatable
	static const luaL_Reg TilesContainer_lib_m[] = {
		{ "eachTile", l_TilesContainer_eachTile },
		{ "getPositions", l_TilesContainer_getPositions},
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
		lua_pushinteger(L, tileEffect.tileIndex);
		lua_pushnumber(L, tileEffect.effect);
		lua_call(L, 2, 0);
	}
	return 0;
}

int l_TilesContainer_getPositions(lua_State* L)
{
	game::map::Map& map = game::map::lua::map::getMap(L);
	TilesContainer& tilesContainer = getTilesContainer(L, 1);
	std::vector<flat::Vector2> positions;
	positions.reserve(tilesContainer.size());
	for (TileEffect& tileEffect : tilesContainer)
	{
		positions.push_back(map.getTileXY(tileEffect.tileIndex));
	}
	flat::lua::table::pushVector<flat::Vector2>(L, positions);
	return 1;
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

} // game::map::brush::lua


