#include <algorithm>
#include "timercontainer.h"
#include "timer.h"
#include "lua/timer.h"

namespace game
{
namespace timer
{

TimerContainer::~TimerContainer()
{
	clearTimers();
}

Timer* TimerContainer::add(float beginTime, float duration, int onUpdate, int onEnd, bool loop)
{
	Timer* timer = m_timerPool.create(beginTime, duration, onUpdate, onEnd, loop);
	m_pendingTimers.push_back(timer);
	return timer;
}

bool TimerContainer::stop(Timer*& timer)
{
	std::deque<Timer*>::iterator it = std::find(m_timers.begin(), m_timers.end(), timer);
	if (it != m_timers.end())
	{
		m_timerPool.destroy(timer);
		*it = nullptr;
		timer = nullptr;
		return true;
	}
	else
	{
		return false;
	}
}

void TimerContainer::updateTimers(lua_State* L, float currentTime)
{
	// insert pending timers
	for (Timer* timer : m_pendingTimers)
	{
		std::deque<Timer*>::iterator it = std::upper_bound(m_timers.begin(), m_timers.end(), timer, &TimerContainer::compareTimersByTimeout);
		m_timers.insert(it, timer);
	}
	m_pendingTimers.clear();

	// update timers
	std::deque<Timer*>::iterator lastStoppedTimerIt = m_timers.begin();
	std::deque<Timer*>::iterator end = m_timers.end();
	std::vector<Timer*> loopingTimers;
	for (std::deque<Timer*>::iterator it = m_timers.begin(); it != end; ++it)
	{
		Timer* timer = *it;
		FLAT_ASSERT(timer != nullptr);
		float timeOut = timer->getTimeOut();
		if (currentTime >= timeOut)
		{
			// update one last time before dying
			lua::callTimerUpdate(L, timer, timeOut);
			lua::callTimerEnd(L, timer);
			
			lastStoppedTimerIt = it + 1;
			if (timer->getLoop())
			{
				loopingTimers.push_back(timer);
			}
			else
			{
				m_timerPool.destroy(timer);
			}
		}
		else
		{
			lua::callTimerUpdate(L, timer, currentTime);
		}
	}
	m_timers.erase(m_timers.begin(), lastStoppedTimerIt);

	// push looping timers back
	for (Timer* timer : loopingTimers)
	{
		timer->setBeginTime(currentTime);
		std::deque<Timer*>::iterator it = std::upper_bound(m_timers.begin(), m_timers.end(), timer, &TimerContainer::compareTimersByTimeout);
		m_timers.insert(it, timer);
	}
}

void TimerContainer::clearTimers()
{
	for (Timer* timer : m_timers)
	{
		m_timerPool.destroy(timer);
	}
	m_timers.clear();

	for (Timer* timer : m_pendingTimers)
	{
		m_timerPool.destroy(timer);
	}
	m_pendingTimers.clear();
}

bool TimerContainer::compareTimersByTimeout(const Timer* a, const Timer* b)
{
	return a->getTimeOut() < b->getTimeOut();
}

} // timer
} // game
