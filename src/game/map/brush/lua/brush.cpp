#include <flat.h>
#include "brush.h"
#include "../conebrush.h"
#include "../spherebrush.h"
#include "../../../game.h"
#include "../../../states/basemapstate.h"

namespace game
{
namespace map
{
namespace brush
{
namespace lua
{

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	// Brush metatable
	static const luaL_Reg Brush_lib_m[] = {
		{"setRadius",    l_Brush_setRadius},
		{"setEdgeWidth", l_Brush_setEdgeWidth},

		{"getTiles",     l_Brush_getTiles},

		{ nullptr, nullptr }
	};

	flat::lua::ClassRegistry::registerClass<Brush>("CG.Brush", L, Brush_lib_m);

	// Brush static methods
	static const luaL_Reg Brush_lib_s[] = {
		{"cone", l_Brush_cone},
		{"sphere", l_Brush_sphere},

		{nullptr, nullptr}
	};

	luaL_newlib(L, Brush_lib_s);
	lua_setglobal(L, "Brush");

	return 0;
}

int l_Brush_setRadius(lua_State * L)
{
	Brush& brush = getBrush(L, 1);
	float radius = static_cast<float>(luaL_checknumber(L, 2));
	brush.setRadius(radius);
	return 0;
}

int l_Brush_setEdgeWidth(lua_State * L)
{
	Brush& brush = getBrush(L, 1);
	float edgeWidth = static_cast<float>(luaL_checknumber(L, 2));
	brush.setEdgeWidth(edgeWidth);
	return 0;
}

int l_Brush_getTiles(lua_State* L)
{
	Brush& brush = getBrush(L, 1);
	float x = static_cast<float>(luaL_checknumber(L, 2));
	float y = static_cast<float>(luaL_checknumber(L, 3));
	flat::Vector2 center(x, y);
	float minEffect = static_cast<float>(luaL_optnumber(L, 4, 0.f));
	Game& game = flat::lua::getGame(L).to<Game>();
	states::BaseMapState& baseMapState = game.getStateMachine().getState()->to<states::BaseMapState>();
	Map& map = baseMapState.getMap();
	std::map<map::Tile*, float> tiles;
	brush.getTiles(map, center, tiles, minEffect);
	lua_createtable(L, 0, static_cast<int>(tiles.size()));
	for (const std::pair<map::Tile*, float>& tile : tiles)
	{
		lua_pushlightuserdata(L, tile.first);
		lua_pushnumber(L, tile.second);
		lua_rawset(L, -3);
	}
	return 1;
}

int l_Brush_cone(lua_State* L)
{
	flat::lua::SharedCppReference<Brush>::pushNew(L, new ConeBrush());
	return 1;
}

int l_Brush_sphere(lua_State* L)
{
	flat::lua::SharedCppReference<Brush>::pushNew(L, new SphereBrush());
	return 1;
}

// private
Brush& getBrush(lua_State* L, int index)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	return *flat::lua::SharedCppReference<Brush>::getSharedPointer(L, index).get();
}

} // lua
} // brush
} // map
} // game


