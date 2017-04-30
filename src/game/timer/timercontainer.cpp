#include <algorithm>
#include "timercontainer.h"
#include "timer.h"
#include "lua/timer.h"

namespace game
{
namespace timer
{

TimerContainer::TimerContainer()
{
	
}

TimerContainer::~TimerContainer()
{
	for (const Timer* timer : m_timers)
	{
		delete timer;
	}
}

void TimerContainer::add(const Timer* timer)
{
	std::deque<const Timer*>::iterator it = std::upper_bound(m_timers.begin(), m_timers.end(), timer, &TimerContainer::compareTimersByTimeout);
	m_timers.insert(it, timer);
}

bool TimerContainer::stop(const Timer* timer)
{
	std::deque<const Timer*>::iterator it = std::find(m_timers.begin(), m_timers.end(), timer);
	if (it != m_timers.end())
	{
		m_timers.erase(it);
		return true;
	}
	else
	{
		return false;
	}
}

void TimerContainer::updateTimers(lua_State* L, float currentTime)
{
	std::deque<const Timer*>::iterator lastStoppedTimerIt = m_timers.begin();
	std::deque<const Timer*>::iterator end = m_timers.end();
	std::deque<const Timer*> loopingTimers;
	for (std::deque<const Timer*>::iterator it = m_timers.begin(); it != end; ++it)
	{
		const Timer* timer = *it;
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
				delete timer;
			}
		}
		else
		{
			lua::callTimerUpdate(L, timer, currentTime);
		}
	}
	m_timers.erase(m_timers.begin(), lastStoppedTimerIt);
	for (const Timer* timer : loopingTimers)
	{
		timer->setBeginTime(currentTime);
		add(timer);
	}
}

void TimerContainer::clearTimers()
{
	m_timers.clear();
}

bool TimerContainer::compareTimersByTimeout(const Timer* a, const Timer* b)
{
	return a->getTimeOut() < b->getTimeOut();
}

} // timer
} // game
