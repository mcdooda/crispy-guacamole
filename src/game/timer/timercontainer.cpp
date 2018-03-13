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

Timer* TimerContainer::add(float duration, const flat::lua::SharedLuaReference<LUA_TFUNCTION>& onUpdate, const flat::lua::SharedLuaReference<LUA_TFUNCTION>& onEnd, bool loop)
{
	const float beginTime = m_clock->getTime();
	Timer* timer = m_timerPool.create(beginTime, duration, onUpdate, onEnd, loop);
	FLAT_ASSERT(timer != nullptr);
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

void TimerContainer::updateTimers(lua_State* L)
{
	FLAT_ASSERT(m_clock != nullptr);
	// remove stopped timers
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
	for (Timer* timer : m_pendingTimers)
	{
		std::deque<Timer*>::iterator it = std::upper_bound(m_timers.begin(), m_timers.end(), timer, &TimerContainer::compareTimersByTimeout);
		m_timers.insert(it, timer);
	}
	m_pendingTimers.clear();

	// update timers
	const float time = m_clock->getTime();
	std::deque<Timer*>::iterator lastStoppedTimerIt = m_timers.begin();
	std::deque<Timer*>::iterator end = m_timers.end();
	std::vector<Timer*> loopingTimers;
	for (std::deque<Timer*>::iterator it = m_timers.begin(); it != end;)
	{
		Timer* timer = *it;
		if (timer != nullptr)
		{
			float timeOut = timer->getTimeOut();
			if (time >= timeOut)
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
				lua::callTimerUpdate(L, timer, time);
			}
			++it;
		}
		else
		{
			assert(lastStoppedTimerIt != it);
			it = m_timers.erase(it);
		}
	}
	std::deque<Timer*> tmpTimers = m_timers;
	m_timers.erase(m_timers.begin(), lastStoppedTimerIt);

	// push looping timers back
	for (Timer* timer : loopingTimers)
	{
		if (std::find(tmpTimers.begin(), tmpTimers.end(), timer) != tmpTimers.end())
		{
			timer->setBeginTime(time);
			std::deque<Timer*>::iterator it = std::upper_bound(m_timers.begin(), m_timers.end(), timer, &TimerContainer::compareTimersByTimeout);
			m_timers.insert(it, timer);
		}
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
