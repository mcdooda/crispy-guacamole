#ifndef GAME_TIMER_TIMER_H
#define GAME_TIMER_TIMER_H

namespace game
{
namespace timer
{

class Timer
{
	public:
		Timer(float beginTime, float duration, int onUpdate, int onEnd, bool loop);
		
		inline void setBeginTime(float beginTime) const { m_beginTime = beginTime; }
		inline float getBeginTime() const { return m_beginTime; }

		inline float getDuration() const { return m_duration; }
		inline float getTimeOut() const { return m_beginTime + m_duration; }
		inline int getOnUpdate() const { return m_onUpdate; }
		inline int getOnEnd() const { return m_onEnd; }
		inline bool getLoop() const { return m_loop; }
		
		inline float getElapsedTime(float currentTime) const { return currentTime - m_beginTime; }
		inline bool isFinished(float currentTime) const { return currentTime >= getTimeOut(); }
		
	private:
		mutable float m_beginTime;
		float m_duration;
		int m_onUpdate;
		int m_onEnd;
		bool m_loop;
};

} // timer
} // game

#endif // GAME_TIMER_TIMER_H



