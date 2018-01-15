#ifndef GAME_STATES_LUA_MAPEDITOR_H
#define GAME_STATES_LUA_MAPEDITOR_H

#include <flat.h>

namespace game
{
namespace states
{
namespace lua
{
namespace mapeditor
{

int open(lua_State* L);

int l_MapEditor_newMap(lua_State* L);
int l_MapEditor_openMap(lua_State* L);

} // mapeditor
} // lua
} // states
} // game

#endif // GAME_STATES_LUA_MAPEDITOR_H


