#include "timer.h"

namespace game
{
namespace timer
{

Timer::Timer(float beginTime, float duration, const flat::lua::SharedLuaReference<LUA_TFUNCTION>& onUpdate, const flat::lua::SharedLuaReference<LUA_TFUNCTION>& onEnd, bool loop) :
	m_beginTime(beginTime),
	m_duration(duration),
	m_onUpdate(onUpdate),
	m_onEnd(onEnd),
	m_loop(loop)
{
	
}

void Timer::reset(lua_State* L)
{
	m_onUpdate.reset(L);
	m_onEnd.reset(L);
}

} // timer
} // game
