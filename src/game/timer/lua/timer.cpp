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
		{"start",          l_Timer_start},
		{"stop",           l_Timer_stop},
		{"getElapsedTime", l_Timer_getElapsedTime},
		{"onUpdate",       l_Timer_onUpdate},
		{"onEnd",          l_Timer_onEnd},
		
		{nullptr, nullptr}
	};
	game.lua->registerClass<LuaTimer>("CG.Timer", Timer_lib_m);
	
	static const luaL_Reg Timer_lib_s[] = {
		{"new", l_Timer_new},
		
		{nullptr, nullptr}
	};
	luaL_newlib(L, Timer_lib_s);
	lua_setglobal(L, "Timer");
	
	return 0;
}

int l_Timer_new(lua_State* L)
{
	TimerContainer& timerContainer = getTimerContainer(L);
	pushTimer(L, timerContainer.add());
	return 1;
}

int l_Timer_start(lua_State* L)
{
	TimerContainer& timerContainer = getTimerContainer(L);
	Timer* timer = getTimer(L, 1);
	if (!timerContainer.exists(timer))
	{
		luaL_error(L, "Timer does not exists anymore, did you stop it?");
	}
	const float duration = static_cast<float>(luaL_checknumber(L, 2));
	const bool loop = lua_toboolean(L, 3) == 1;
	timer->setDuration(duration);
	timer->setBeginTime(timerContainer.getClock().getTime());
	timer->setLoop(loop);
	callTimerUpdate(L, timer, timerContainer.getClock().getTime());
	return 0;
}

int l_Timer_stop(lua_State* L)
{
	TimerContainer& timerContainer = getTimerContainer(L);
	Timer* timer = getTimer(L, 1);
	lua_pushboolean(L, timerContainer.stop(timer));
	return 1;
}

int l_Timer_getElapsedTime(lua_State* L)
{
	TimerContainer& timerContainer = getTimerContainer(L);
	const float time = timerContainer.getClock().getTime();
	Timer* timer = getTimer(L, 1);
	if (!timerContainer.exists(timer))
	{
		luaL_error(L, "Timer does not exists anymore, did you stop it?");
	}
	lua_pushnumber(L, timer->getElapsedTime(time));
	return 1;
}

int l_Timer_onUpdate(lua_State* L)
{
	TimerContainer& timerContainer = getTimerContainer(L);
	Timer* timer = getTimer(L, 1);	
	if (!timerContainer.exists(timer))
	{
		luaL_error(L, "Timer does not exists anymore, did you stop it?");
	}
	flat::lua::SharedLuaReference<LUA_TFUNCTION> onUpdate;
	onUpdate.setIfNotNil(L, 2);
	timer->setOnUpdate(onUpdate);
	return 0;
}

int l_Timer_onEnd(lua_State* L)
{
	TimerContainer& timerContainer = getTimerContainer(L);
	Timer* timer = getTimer(L, 1);	
	if (!timerContainer.exists(timer))
	{
		luaL_error(L, "Timer does not exists anymore, did you stop it?");
	}
	flat::lua::SharedLuaReference<LUA_TFUNCTION> onEnd;
	onEnd.setIfNotNil(L, 2);
	timer->setOnEnd(onEnd);
	return 0;
}

void callTimerUpdate(lua_State* L, Timer* timer, float currentTime)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	const flat::lua::SharedLuaReference<LUA_TFUNCTION>& onUpdate = timer->getOnUpdate();
	if (!onUpdate.isEmpty())
	{
		onUpdate.push(L);
		pushTimer(L, timer);
		float elapsedTime = timer->getElapsedTime(currentTime);
		lua_pushnumber(L, elapsedTime);
		lua_call(L, 2, 0);
	}
}

void callTimerEnd(lua_State* L, Timer* timer)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	const flat::lua::SharedLuaReference<LUA_TFUNCTION>& onEnd = timer->getOnEnd();
	if (!onEnd.isEmpty())
	{
		onEnd.push(L);
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


