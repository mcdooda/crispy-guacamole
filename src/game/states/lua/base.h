#ifndef GAME_STATES_LUA_BASE_H
#define GAME_STATES_LUA_BASE_H

#include <flat.h>

namespace game
{
namespace states
{
class BaseState;
namespace lua
{
namespace base
{

BaseState& getBaseState(lua_State* L);

} // base
} // lua
} // states
} // game

#endif // GAME_STATES_LUA_BASE_H


