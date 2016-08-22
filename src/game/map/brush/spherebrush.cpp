#include "spherebrush.h"

namespace game
{
namespace map
{
namespace brush
{

	float SphereBrush::getEffect(const flat::Vector2& positionFromCenter) const
	{
		FLAT_ASSERT(m_radius > 0.f);
		return 1.f - positionFromCenter.lengthSquared() / (m_radius * m_radius); // 1 - (length / m_size) ^ 2
	}

} // brush
} // map
} // map



