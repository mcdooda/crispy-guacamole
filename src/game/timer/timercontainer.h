#ifndef GAME_TIMER_TIMERCONTAINER_H
#define GAME_TIMER_TIMERCONTAINER_H

#include <deque>
#include <flat.h>

namespace game
{
namespace timer
{
class Timer;

class TimerContainer
{
	public:
		TimerContainer();
		~TimerContainer();
		
		void add(const Timer* timer);
		bool stop(const Timer* timer);
		
		void updateTimers(lua_State* L, float currentTime);
		
		void clearTimers();
		
	private:
		static bool compareTimersByTimeout(const Timer* a, const Timer* b);
		
		std::deque<const Timer*> m_timers;
};

} // timer
} // game

#endif // GAME_TIMER_TIMERCONTAINER_H



