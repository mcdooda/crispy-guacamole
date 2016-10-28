#include "animationdescription.h"

namespace game
{
namespace entity
{
namespace component
{
namespace sprite
{

AnimationDescription::AnimationDescription(int line, int numFrames, float frameDuration) :
	m_line(line),
	m_numFrames(numFrames),
	m_frameDuration(frameDuration)
{
	
}

} // sprite
} // component
} // entity
} // game



