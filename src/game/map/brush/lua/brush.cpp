#include <flat.h>
#include "brush.h"
#include "tilescontainer.h"
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

using LuaBrush = flat::lua::SharedCppReference<Brush>;

int open(Game& game)
{
	lua_State* L = game.lua->state;
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	// Brush metatable
	static const luaL_Reg Brush_lib_m[] = {
		{"setRadius",    l_Brush_setRadius},
		{"setEdgeWidth", l_Brush_setEdgeWidth},

		{"getTiles",     l_Brush_getTiles},

		{ nullptr, nullptr }
	};
	game.lua->registerClass<LuaBrush>("CG.Brush", Brush_lib_m);

	// Brush static methods
	static const luaL_Reg Brush_lib_s[] = {
		{"cone", l_Brush_cone},
		{"sphere", l_Brush_sphere},

		{nullptr, nullptr}
	};

	luaL_newlib(L, Brush_lib_s);
	lua_setglobal(L, "Brush");

	openTilesContainer(game);

	return 0;
}

int l_Brush_setRadius(lua_State* L)
{
	Brush& brush = getBrush(L, 1);
	float radius = static_cast<float>(luaL_checknumber(L, 2));
	brush.setRadius(radius);
	return 0;
}

int l_Brush_setEdgeWidth(lua_State* L)
{
	Brush& brush = getBrush(L, 1);
	float edgeWidth = static_cast<float>(luaL_checknumber(L, 2));
	brush.setEdgeWidth(edgeWidth);
	return 0;
}

int l_Brush_getTiles(lua_State* L)
{
	Brush& brush = getBrush(L, 1);
	flat::Vector2& center = flat::lua::getVector2(L, 2);
	float minEffect = static_cast<float>(luaL_optnumber(L, 3, 0.f));
	Game& game = flat::lua::getFlatAs<Game>(L);
	states::BaseMapState& baseMapState = game.getStateMachine().getState()->to<states::BaseMapState>();
	Map& map = baseMapState.getMap();
	TilesContainer* tilesContainer = new TilesContainer();
	brush.getTiles(map, center, *tilesContainer, minEffect);
	pushTilesContainer(L, tilesContainer);
	return 1;
}

int l_Brush_cone(lua_State* L)
{
	LuaBrush::pushNew(L, new ConeBrush());
	return 1;
}

int l_Brush_sphere(lua_State* L)
{
	LuaBrush::pushNew(L, new SphereBrush());
	return 1;
}

// private
Brush& getBrush(lua_State* L, int index)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	return LuaBrush::get(L, index);
}

} // lua
} // brush
} // map
} // game


