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

#ifdef FLAT_DEBUG
int l_Game_debug_setTimeSpeed(lua_State* L);
int l_Game_debug_pause(lua_State* L);
int l_Game_debug_resume(lua_State* L);
int l_Game_debug_pauseNextFrame(lua_State* L);

int l_Game_debug_getNumTimers(lua_State* L);
int l_Game_debug_getNumFrameTimers(lua_State* L);
int l_Game_debug_getNumPendingTimers(lua_State* L);

int l_Game_debug_reloadComponent(lua_State* L);
int l_Game_debug_removeComponent(lua_State* L);
#endif // FLAT_DEBUG

int l_Game_openMap(lua_State* L);

int l_Game_setGhostEntity(lua_State* L);
int l_Game_clearGhostEntity(lua_State* L);

int l_Game_setCameraCenter(lua_State* L);
int l_Game_getCameraCenter(lua_State* L);
int l_Game_setCameraZoom(lua_State* L);
int l_Game_lockCamera(lua_State* L);
int l_Game_unlockCamera(lua_State* L);
int l_Game_convertToCameraPosition(lua_State* L);

} // game
} // lua
} // states
} // game

#endif // GAME_STATES_LUA_GAME_H


