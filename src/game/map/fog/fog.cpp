#include "map/fog/fog.h"
#include "map/fog/nofog.h"
#include "map/fog/hardfog.h"
#include "map/map.h"
#include "map/displaymanager.h"


namespace game
{
namespace map
{
namespace fog
{

Fog::Fog(const map::Map& map) :
	m_map(map)
{

}

std::unique_ptr<Fog> Fog::createFogFromType(FogType fogType, const map::Map& map)
{
	switch (fogType)
	{
	case FogType::NONE:
		return std::make_unique<NoFog>(map);
	case FogType::HARD:
		return std::make_unique<HardFog>(map);
	default:
		FLAT_ASSERT(false);
	}
	return nullptr;
}

void Fog::init()
{

}

void Fog::discover(const flat::Vector2& xy, float radius)
{
	if (!needsDiscovering())
	{
		return;
	}

	const int minX = static_cast<int>(std::round(xy.x - radius));
	const int maxX = static_cast<int>(std::round(xy.x + radius));
	const int minY = static_cast<int>(std::round(xy.y - radius));
	const int maxY = static_cast<int>(std::round(xy.y + radius));
	const float radius2 = flat::square(radius);
	for (int x = minX; x <= maxX; ++x)
	{
		for (int y = minY; y <= maxY; ++y)
		{
			flat::Vector2 tilePosition(x, y);
			const float distance2 = flat::distance2(tilePosition, xy);
			if (distance2 <= radius2)
			{
				const float distance = std::sqrt(distance2);
				const float level = std::min((radius - distance), 1.f);
				discoverTile(x, y, level);
			}
		}
	}
}

} // fog
} // map
} // game