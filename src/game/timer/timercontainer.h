#ifndef GAME_TIMER_TIMERCONTAINER_H
#define GAME_TIMER_TIMERCONTAINER_H

#include <deque>
#include <vector>
#include <flat.h>
#include "timer.h"

namespace game
{
namespace timer
{

class TimerContainer
{
	public:
		TimerContainer() = default;
		TimerContainer(const TimerContainer&) = delete;
		TimerContainer(TimerContainer&&) = delete;
		~TimerContainer();

		void operator=(const TimerContainer&) = delete;
		
		Timer* add(float beginTime, float duration, int onUpdate, int onEnd, bool loop);
		bool stop(Timer*& timer);
		
		void updateTimers(lua_State* L, float currentTime);
		
		void clearTimers();
		
	private:
		static bool compareTimersByTimeout(const Timer* a, const Timer* b);
		
		flat::containers::Pool<Timer, 256> m_timerPool;
		std::vector<Timer*> m_pendingTimers;
		std::deque<Timer*> m_timers;
};

} // timer
} // game

#endif // GAME_TIMER_TIMERCONTAINER_H



