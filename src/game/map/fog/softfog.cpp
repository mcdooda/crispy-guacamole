#include "map/fog/softfog.h"
#include "map/map.h"

namespace game
{
namespace map
{
namespace fog
{

SoftFog::SoftFog(const map::Map& map) : HardFog(map)
{

}

Fog::FogType SoftFog::getFogType() const
{
	return FogType::SOFT;
}

bool SoftFog::isTileDiscovered(TileIndex tileIndex) const
{
	return m_tileDiscoveryLevels[tileIndex] > TILE_DISCOVERED_LEVEL;
}

void SoftFog::preUpdate()
{
	const flat::video::Color tileDiscoveredColor(static_cast<float>(TILE_DISCOVERED_LEVEL) / 255.f);
	for (const TileIndex tileIndex : m_frameObservedTiles)
	{
		FLAT_ASSERT(m_tileDiscoveryLevels[tileIndex] > 0);
		if (m_tileDiscoveryLevels[tileIndex] > TILE_DISCOVERED_LEVEL)
		{
			m_tileDiscoveryLevels[tileIndex] = TILE_DISCOVERED_LEVEL;
			m_observedTiles[tileIndex].getSprite().setColor(tileDiscoveredColor);

			const PropIndex propIndex = m_map.getTilePropIndex(tileIndex);
			if (isValidProp(propIndex))
			{
				m_observedProps[propIndex].getSprite().setColor(tileDiscoveredColor);
			}
		}
	}
	m_frameObservedTiles.clear();
}

} // fog
} // map
} // game