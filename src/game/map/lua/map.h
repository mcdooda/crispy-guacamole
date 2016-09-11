#ifndef GAME_MAP_LUA_MAP_H
#define GAME_MAP_LUA_MAP_H

#include <flat.h>

namespace game
{
namespace map
{
namespace lua
{

int open(lua_State* L);

int l_Map_load(lua_State* L);
int l_Map_save(lua_State* L);

}
}
}

#endif // GAME_MAP_LUA_MAP_H


