#ifndef GAME_MAP_LUA_MAP_H
#define GAME_MAP_LUA_MAP_H

#include <flat.h>

namespace game
{
namespace map
{
class Zone;
namespace lua
{
namespace map
{

int open(lua_State* L);

int l_Map_getName(lua_State* L);
int l_Map_load(lua_State* L);
int l_Map_save(lua_State* L);

int l_Map_getNumEntities(lua_State* L);

int l_Map_getEntitiesInRange(lua_State* L);

int l_Map_getZone(lua_State* L);

} // map
} // lua
} // map
} // game

#endif // GAME_MAP_LUA_MAP_H


