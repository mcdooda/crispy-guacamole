#ifndef GAME_TIMER_LUA_TIMER_H
#define GAME_TIMER_LUA_TIMER_H

#include <flat.h>

namespace game
{
namespace timer
{
class Timer;
class TimerContainer;
namespace lua
{

int open(lua_State* L);

int l_Timer_start(lua_State* L);
int l_Timer_stop(lua_State* L);
int l_Timer_getElapsedTime(lua_State* L);

void callTimerUpdate(lua_State* L, Timer* timer, float currentTime);
void callTimerEnd(lua_State* L, Timer* timer);

// private
Timer* getTimer(lua_State* L, int index);
void pushTimer(lua_State* L, Timer* timer);

TimerContainer& getTimerContainer(lua_State* L);

} // lua
} // timer
} // game

#endif // GAME_TIMER_LUA_TIMER_H



