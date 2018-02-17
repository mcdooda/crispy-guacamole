#ifndef GAME_MAP_LUA_ZONE_H
#define GAME_MAP_LUA_ZONE_H

#include <flat.h>

namespace game
{
class Game;
namespace map
{
class Zone;
namespace lua
{
namespace zone
{

int open(Game& game);

int l_Zone_eq(lua_State* L);

int l_Zone_getEntities(lua_State* L);
int l_Zone_getEntitiesCount(lua_State* L);

int l_Zone_getCenter(lua_State* L);

int l_Zone_isTileInside(lua_State* L);

Zone* getZone(lua_State* L, int index);
void pushZone(lua_State* L, const std::shared_ptr<Zone>& zone);

} // zone
} // lua
} // map
} // game

#endif // GAME_MAP_LUA_ZONE_H


