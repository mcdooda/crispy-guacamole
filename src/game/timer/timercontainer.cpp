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

size_t TimerContainer::size() const
{
	return m_timers.size() + m_frameTimers.size();
}

Timer* TimerContainer::add()
{
	Timer* timer = m_timerPool.create();
	FLAT_ASSERT(timer != nullptr);
	m_pendingTimers.push_back(timer);
	return timer;
}

bool TimerContainer::exists(Timer* timer)
{
	std::deque<Timer*>::iterator it = std::find(m_timers.begin(), m_timers.end(), timer);
	if (it != m_timers.end())
	{
		return true;
	}
	else
	{
		std::vector<Timer*>::iterator it = std::find(m_pendingTimers.begin(), m_pendingTimers.end(), timer);
		return it != m_pendingTimers.end();
	}
}

bool TimerContainer::stop(Timer*& timer)
{
	std::deque<Timer*>::iterator it = std::find(m_timers.begin(), m_timers.end(), timer);
	if (it != m_timers.end())
	{
		m_timerPool.destroy(*it);
		*it = nullptr;
		return true;
	}
	else
	{
		std::vector<Timer*>::iterator it = std::find(m_pendingTimers.begin(), m_pendingTimers.end(), timer);
		if (it != m_pendingTimers.end())
		{
			m_timerPool.destroy(*it);
			m_pendingTimers.erase(it);
			return true;
		}
		return false;
	}
}

void TimerContainer::updateTimers(lua_State* L)
{
	FLAT_ASSERT(m_clock != nullptr);
	const float time = m_clock->getTime();
	size_t size = m_timers.size();
	m_timers.erase(
	    std::remove(
			m_timers.begin(),
			m_timers.end(),
			nullptr
			),
		m_timers.end()
	);
	FLAT_ASSERT(std::is_sorted(m_timers.begin(), m_timers.end(), &TimerContainer::compareTimersByTimeout));
	// insert pending timers
	for (std::vector<Timer*>::iterator it = m_pendingTimers.begin(); it != m_pendingTimers.end();)
	{
		Timer* timer = *it;
		if (timer->getBeginTime() >= 0)
		{
			if (!timer->getOnUpdate().isEmpty())
			{
				m_frameTimers.push_back(timer);
			}
			else
			{
				std::deque<Timer*>::iterator sortedIterator = std::upper_bound(m_timers.begin(), m_timers.end(), timer, &TimerContainer::compareTimersByTimeout);
				m_timers.insert(sortedIterator, timer);
			}
			it = m_pendingTimers.erase(it);
		}
		else
		{
			it++;
		}
	}

	// update timers
	for (std::deque<Timer*>::iterator it = m_timers.begin(); it != m_timers.end(); it++)
	{
		Timer* timer = *it;
		if (timer != nullptr)
		{
			const float timeOut = timer->getTimeOut();
			if (time >= timeOut)
			{
				lua::callTimerEnd(L, timer);
				if (timer->getLoop())
				{
					timer->setBeginTime(time);
				}
				else
				{
					m_timerPool.destroy(timer);
					*it = nullptr;
				}
			}
			else
			{
				break;
			}
		}
	}
	std::sort(m_timers.begin(), m_timers.end(), &TimerContainer::compareTimersByTimeout);

	// update frame timers
	for (std::vector<Timer*>::iterator it = m_frameTimers.begin(); it != m_frameTimers.end();)
	{
		Timer* timer = *it;
		const float timeOut = timer->getTimeOut();
		if (time >= timeOut)
		{
			// update one last time before dying
			lua::callTimerUpdate(L, timer, timeOut);
			if (timer->getLoop())
			{
				timer->setBeginTime(time);
				it++;
			}
			else
			{
				lua::callTimerEnd(L, timer);
				m_timerPool.destroy(timer);
				it = m_frameTimers.erase(it);
			}
		}
		else
		{
			lua::callTimerUpdate(L, timer, time);
			it++;
		}
	}
}

void TimerContainer::clearTimers()
{
	for (Timer* timer : m_timers)
	{
		if (timer != nullptr)
		{
			m_timerPool.destroy(timer);
		}
	}
	m_timers.clear();

	for (Timer* timer : m_frameTimers)
	{
		if (timer != nullptr)
		{
			m_timerPool.destroy(timer);
		}
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
	if (a == nullptr)
		return true;
	if (b == nullptr)
		return false;
	return a->getTimeOut() < b->getTimeOut();
}

} // timer
} // game
