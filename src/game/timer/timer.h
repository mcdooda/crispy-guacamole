#ifndef GAME_TIMER_TIMER_H
#define GAME_TIMER_TIMER_H

#include <flat.h>

namespace game
{
namespace timer
{

class Timer
{
	public:
		Timer();
		
		inline void setBeginTime(float beginTime) const { m_beginTime = beginTime; }
		inline float getBeginTime() const { return m_beginTime; }

		inline void setOnUpdate(const flat::lua::SharedLuaReference<LUA_TFUNCTION>& onUpdate) { m_onUpdate = onUpdate; }
		inline void setOnEnd(const flat::lua::SharedLuaReference<LUA_TFUNCTION>& onEnd) { m_onEnd = onEnd; }

		inline void setDuration(float duration) { m_duration = duration; }
		inline float getDuration() const { return m_duration; }
		inline float getTimeOut() const { return m_beginTime + m_duration; }

		inline const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getOnUpdate() const { return m_onUpdate; }
		inline const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getOnEnd() const { return m_onEnd; }

		inline void setLoop(bool loop) { m_loop = loop; }
		inline bool getLoop() const { return m_loop; }
		
		inline float getElapsedTime(float currentTime) const { return currentTime - m_beginTime; }
		inline bool isFinished(float currentTime) const { return currentTime >= getTimeOut(); }
		
	private:
		mutable float m_beginTime;
		float m_duration;
		flat::lua::SharedLuaReference<LUA_TFUNCTION> m_onUpdate;
		flat::lua::SharedLuaReference<LUA_TFUNCTION> m_onEnd;
		bool m_loop;
};

} // timer
} // game

#endif // GAME_TIMER_TIMER_H



