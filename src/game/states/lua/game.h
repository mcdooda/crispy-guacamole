#ifndef GAME_STATES_LUA_GAME_H
#define GAME_STATES_LUA_GAME_H

#include <flat.h>

namespace game
{
namespace states
{
class BaseState;
namespace lua
{
namespace game
{

int open(lua_State* L);

int l_Game_getTime(lua_State* L);

int l_Game_openMap(lua_State* L);

// private
BaseState& getBaseState(lua_State* L);

} // game
} // lua
} // states
} // game

#endif // GAME_STATES_LUA_GAME_H


