#include "conebrush.h"

namespace game
{
namespace map
{
namespace brush
{

float ConeBrush::getEffect(const flat::Vector2& positionFromCenter) const
{
	FLAT_ASSERT(m_radius > 0.f);
	return 1.f - flat::length(positionFromCenter) / m_radius;
}

} // brush
} // map
} // map



