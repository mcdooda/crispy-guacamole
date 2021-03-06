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

int l_game_getTime(lua_State* L);
int l_game_getDT(lua_State* L);

int l_game_Timer(lua_State* L);

#ifdef FLAT_DEBUG
int l_game_debug_setTimeSpeed(lua_State* L);
int l_game_debug_pause(lua_State* L);
int l_game_debug_resume(lua_State* L);
int l_game_debug_pauseNextFrame(lua_State* L);

int l_game_debug_getNumTimers(lua_State* L);
int l_game_debug_getNumFrameTimers(lua_State* L);

int l_game_debug_reloadComponent(lua_State* L);
int l_game_debug_removeComponent(lua_State* L);

int l_game_debug_setVolume(lua_State* L);
#endif // FLAT_DEBUG

int l_game_openMap(lua_State* L);

int l_game_setGhostEntity(lua_State* L);
int l_game_clearGhostEntity(lua_State* L);

int l_game_setCameraCenter(lua_State* L);
int l_game_getCameraCenter(lua_State* L);
int l_game_setCameraZoom(lua_State* L);
int l_game_lockCamera(lua_State* L);
int l_game_unlockCamera(lua_State* L);
int l_game_convertToCameraPosition(lua_State* L);

} // game
} // lua
} // states
} // game

#endif // GAME_STATES_LUA_GAME_H


