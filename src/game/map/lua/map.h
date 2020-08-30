#ifndef GAME_MAP_LUA_MAP_H
#define GAME_MAP_LUA_MAP_H

#include <flat.h>

namespace game
{
namespace states
{
class BaseMapState;
}
namespace map
{
class Zone;
class Map;
namespace lua
{
namespace map
{

int open(lua_State* L);

int l_Map_getName(lua_State* L);
int l_Map_load(lua_State* L);
int l_Map_save(lua_State* L);

int l_Map_reset(lua_State* L);

#ifdef FLAT_DEBUG
int l_Map_debug_getDrawStats(lua_State* L);
int l_Map_debug_enableNavigabilityDebug(lua_State* L);
int l_Map_debug_enableTileIndicesDebug(lua_State* L);
int l_Map_debug_enableSimplifyPath(lua_State* L);
#endif

int l_Map_getNumEntities(lua_State* L);
int l_Map_getEntitiesInRange(lua_State* L);
int l_Map_eachSelectedEntity(lua_State* L);
int l_Map_selectionChanged(lua_State* L);
int l_Map_getEntitiesOfType(lua_State* L);

int l_Map_getZone(lua_State* L);

int l_Map_getTilePosition(lua_State* L);
int l_Map_getTileZ(lua_State* L);
int l_Map_setTileZ(lua_State* L);
int l_Map_moveTileZBy(lua_State* L);
int l_Map_setTileTemplate(lua_State* L);
int l_Map_eachTile(lua_State* L);

int l_Map_setPropTemplate(lua_State* L);

int l_Map_findPath(lua_State* L);

int l_Map_setFogType(lua_State* L);
int l_Map_getFogType(lua_State* L);

// private
states::BaseMapState& getMapState(lua_State* L);
Map& getMap(lua_State* L);

} // map
} // lua
} // map
} // game

#endif // GAME_MAP_LUA_MAP_H


