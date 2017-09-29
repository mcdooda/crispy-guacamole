#include "timer.h"
#include "../timer.h"
#include "../timercontainer.h"
#include "../../game.h"
#include "../../states/basestate.h"

namespace game
{
namespace timer
{
namespace lua
{

using LuaTimer = flat::lua::SharedCppValue<Timer*>;

int open(Game& game)
{
	lua_State* L = game.lua->state;
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	static const luaL_Reg Timer_lib_m[] = {
		{"stop",           l_Timer_stop},
		{"getElapsedTime", l_Timer_getElapsedTime},
		
		{nullptr, nullptr}
	};
	game.lua->registerClass<LuaTimer>("CT.Timer", Timer_lib_m);
	
	static const luaL_Reg Timer_lib_s[] = {
		{"start", l_Timer_start},
		
		{nullptr, nullptr}
	};
	luaL_newlib(L, Timer_lib_s);
	lua_setglobal(L, "Timer");
	
	return 0;
}

int l_Timer_start(lua_State* L)
{
	float timerDuration = static_cast<float>(luaL_checknumber(L, 1));
	
	int onUpdate = LUA_NOREF;
	if (!lua_isnoneornil(L, 2))
	{
		luaL_checktype(L, 2, LUA_TFUNCTION);
		lua_pushvalue(L, 2);
		onUpdate = luaL_ref(L, LUA_REGISTRYINDEX);
	}
	
	int onEnd = LUA_NOREF;
	if (!lua_isnoneornil(L, 3))
	{
		luaL_checktype(L, 3, LUA_TFUNCTION);
		lua_pushvalue(L, 3);
		onEnd = luaL_ref(L, LUA_REGISTRYINDEX);
	}

	bool loop = lua_toboolean(L, 4) == 1;
	
	TimerContainer& timerContainer = getTimerContainer(L);
	Timer* timer = timerContainer.add(timerDuration, onUpdate, onEnd, loop);
	callTimerUpdate(L, timer, timerContainer.getClock().getTime());
	pushTimer(L, timer);
	return 1;
}

int l_Timer_stop(lua_State* L)
{
	TimerContainer& timerContainer = getTimerContainer(L);
	Timer* timer = getTimer(L, 1);
	if (timerContainer.stop(timer))
	{
		delete timer;
		lua_pushboolean(L, 1);
	}
	else
	{
		lua_pushboolean(L, 0);
	}
	return 1;
}

int l_Timer_getElapsedTime(lua_State* L)
{
	const float time = getTimerContainer(L).getClock().getTime();
	Timer* timer = getTimer(L, 1);
	lua_pushnumber(L, timer->getElapsedTime(time));
	return 1;
}

void callTimerUpdate(lua_State* L, Timer* timer, float currentTime)
{
	int onUpdate = timer->getOnUpdate();
	if (onUpdate != LUA_NOREF)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, onUpdate);
		pushTimer(L, timer);
		float elapsedTime = timer->getElapsedTime(currentTime);
		lua_pushnumber(L, elapsedTime);
		lua_call(L, 2, 0);
	}
}

void callTimerEnd(lua_State* L, Timer* timer)
{
	int onEnd = timer->getOnEnd();
	if (onEnd != LUA_NOREF)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, onEnd);
		pushTimer(L, timer);
		lua_call(L, 1, 0);
	}
}

Timer* getTimer(lua_State* L, int index)
{
	return LuaTimer::get(L, index);
}

void pushTimer(lua_State* L, Timer* timer)
{
	if (timer != nullptr)
	{
		LuaTimer::pushNew(L, timer);
	}
	else
	{
		lua_pushnil(L);
	}
}

TimerContainer& getTimerContainer(lua_State* L)
{
	Game& game = flat::lua::getFlatAs<Game>(L);
	states::BaseState& baseState = game.getStateMachine().getState()->to<states::BaseState>();
	return baseState.getTimerContainer();
}

} // lua
} // timer
} // game


