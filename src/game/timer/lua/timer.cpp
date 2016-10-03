#include "timer.h"
#include "../timer.h"
#include "../timercontainer.h"
#include "../../game.h"

namespace game
{
namespace timer
{
namespace lua
{

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	// Timer metatable
	luaL_newmetatable(L, "CG.Timer");
	// mt.__index = mt
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	
	static const luaL_Reg Timer_lib_m[] = {
		{"stop",           l_Timer_stop},
		{"getElapsedTime", l_Timer_getElapsedTime},
		
		{nullptr, nullptr}
	};
	luaL_setfuncs(L, Timer_lib_m, 0);
	lua_pop(L, 1);
	
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
	
	flat::time::Time* time = flat::lua::getTime(L);
	float currentTime = time->getTime();
	TimerContainer& timerContainer = getTimerContainer(L);
	const Timer* timer = new Timer(currentTime, currentTime + timerDuration, onUpdate, onEnd);
	timerContainer.add(timer);
	callTimerUpdate(L, timer, currentTime);
	return 0;
}

int l_Timer_stop(lua_State* L)
{
	TimerContainer& timerContainer = getTimerContainer(L);
	const Timer* timer = getTimer(L, 1);
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
	flat::time::Time* time = flat::lua::getTime(L);
	float currentTime = time->getTime();
	const Timer* timer = getTimer(L, 1);
	lua_pushnumber(L, timer->getElapsedTime(currentTime));
	return 1;
}

void callTimerUpdate(lua_State* L, const Timer* timer, float currentTime)
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

void callTimerEnd(lua_State* L, const Timer* timer)
{
	int onEnd = timer->getOnEnd();
	if (onEnd != LUA_NOREF)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, onEnd);
		pushTimer(L, timer);
		lua_call(L, 1, 0);
	}
}

const Timer* getTimer(lua_State* L, int index)
{
	return *static_cast<const Timer**>(luaL_checkudata(L, index, "CG.Timer"));
}

void pushTimer(lua_State* L, const Timer* timer)
{
	if (timer != nullptr)
	{
		const Timer** timerPointer = static_cast<const Timer**>(lua_newuserdata(L, sizeof(const Timer*)));
		*timerPointer = timer;
		luaL_getmetatable(L, "CG.Timer");
		lua_setmetatable(L, -2);
	}
	else
	{
		lua_pushnil(L);
	}
}

TimerContainer& getTimerContainer(lua_State* L)
{
	Game& game = flat::lua::getGameAs<Game>(L);
	return game.timerContainer;
}

} // lua
} // timer
} // game


