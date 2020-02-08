#ifndef GAME_STATES_LUA_PATH_H
#define GAME_STATES_LUA_PATH_H

#include <flat.h>

namespace game
{
class Game;
}

namespace game::map::pathfinder
{
class Path;
}

namespace game::map::pathfinder::lua
{
using LuaPath = flat::lua::SharedCppValue<std::shared_ptr<Path>>;

int open(Game& game);
int l_Path_getPoints(lua_State* L);
int l_Path_copy(lua_State* L);
int l_Path_insert(lua_State* L);
int l_Path_eachTile(lua_State* L);
// private
std::shared_ptr<Path> getPath(lua_State* L, int index);
void pushPath(lua_State* L, const std::shared_ptr<Path>& path);

} // game::mape::pathfinder::lua

#endif // GAME_MAP_PATHFINDER_LUA_PATH_H
