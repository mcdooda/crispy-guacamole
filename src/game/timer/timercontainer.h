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

		inline void setClock(const std::shared_ptr<flat::time::Clock>& clock) { m_clock = clock; }
		inline const flat::time::Clock& getClock() const { FLAT_ASSERT(m_clock != nullptr); return *m_clock; }
		
		Timer* add(float duration, int onUpdate, int onEnd, bool loop);
		bool stop(Timer*& timer);
		
		void updateTimers(lua_State* L);
		
		void clearTimers();
		
	private:
		static bool compareTimersByTimeout(const Timer* a, const Timer* b);

	private:
		flat::containers::Pool<Timer, 256> m_timerPool;
		std::vector<Timer*> m_pendingTimers;
		std::deque<Timer*> m_timers;
		std::shared_ptr<flat::time::Clock> m_clock;
};

} // timer
} // game

#endif // GAME_TIMER_TIMERCONTAINER_H



