#include "timer.h"

namespace game
{
namespace timer
{

Timer::Timer(float beginTime, float duration, int onUpdate, int onEnd, bool loop) :
	m_beginTime(beginTime),
	m_duration(duration),
	m_onUpdate(onUpdate),
	m_onEnd(onEnd),
	m_loop(loop)
{
	
}

} // timer
} // game
