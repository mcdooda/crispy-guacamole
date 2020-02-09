#include "path.h"
#include "game.h"
#include "map/pathfinder/path.h"
#include "map/map.h"
#include "map/navigability.h"
#include "lua/table.h"
#include "misc/vector.h"

namespace game::map::pathfinder::lua
{

int open(Game& game)
{
	lua_State* L = game.lua->state;
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	static const luaL_Reg Path_lib_m[] = {
		{"getPoints",    l_Path_getPoints},
		{"copy",         l_Path_copy},
		{"insert",       l_Path_insert},
		{"eachTile",     l_Path_eachTile},
		{nullptr, nullptr}
	};
	game.lua->registerClass<LuaPath>("CG.Path", Path_lib_m);

	// Path static methods
	static const luaL_Reg Path_lib_s[] = {

		{nullptr, nullptr}
	};

	luaL_newlib(L, Path_lib_s);
	lua_setglobal(L, "Path");

	return 0;
}

int l_Path_getPoints(lua_State* L)
{
	std::shared_ptr<Path> path = getPath(L, 1);
	flat::lua::table::pushVector(L, path->getPoints());
	return 1;
}

int l_Path_copy(lua_State* L)
{
	std::shared_ptr<Path> path = getPath(L, 1);
	const std::shared_ptr<Path> copied = std::make_shared<Path>(*path);
	pushPath(L, copied);
	return 1;
}

int l_Path_insert(lua_State* L)
{
	std::shared_ptr<Path> path = getPath(L, 1);
	std::shared_ptr<Path> other = getPath(L, 2);
	path->insertPath(*other);
	return 0;
}

int l_Path_eachTile(lua_State* L)
{
	std::shared_ptr<Path> path = getPath(L, 1);
	luaL_checktype(L, 2, LUA_TFUNCTION);
	for (const flat::Vector2& position : path->getPoints())
	{
		lua_pushvalue(L, 2);
		flat::lua::pushVector2(L, position);
		lua_call(L, 1, 0);
	}
	return 0;
}

void pushPath(lua_State* L, const std::shared_ptr<Path>& path)
{
	if (path != nullptr)
	{
		LuaPath::pushNew(L, path);
	}
	else
	{
		lua_pushnil(L);
	}
}

// private
std::shared_ptr<Path> getPath(lua_State* L, int index)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	return LuaPath::get(L, index);
}

} // game::map::pathfinder::lua


