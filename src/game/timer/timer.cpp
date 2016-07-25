#include "timer.h"

namespace game
{
namespace timer
{

Timer::Timer(float beginTime, float timeOut, int onUpdate, int onEnd) :
	m_beginTime(beginTime),
	m_timeOut(timeOut),
	m_onUpdate(onUpdate),
	m_onEnd(onEnd)
{
	
}

} // timer
} // game
