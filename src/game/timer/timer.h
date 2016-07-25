#ifndef GAME_TIMER_TIMER_H
#define GAME_TIMER_TIMER_H

namespace game
{
namespace timer
{

class Timer
{
	public:
		Timer(float beginTime, float timeOut, int onUpdate, int onEnd);
		
		inline float getBeginTime() const { return m_beginTime; }
		inline float getTimeOut() const { return m_timeOut; }
		inline int getOnUpdate() const { return m_onUpdate; }
		inline int getOnEnd() const { return m_onEnd; }
		
		inline bool isInfinite() const { return m_timeOut <= m_beginTime; }
		inline float getElapsedTime(float currentTime) const { return currentTime - m_beginTime; }
		inline bool isFinished(float currentTime) const { return !isInfinite() && currentTime >= m_timeOut; }
		
	private:
		float m_beginTime;
		float m_timeOut;
		int m_onUpdate;
		int m_onEnd;
};

} // timer
} // game

#endif // GAME_TIMER_TIMER_H



