#include "scopednavigabilityalteration.h"
#include "map.h"

namespace game
{
namespace map
{

ScopedNavigabilityAlteration::ScopedNavigabilityAlteration(Map& map) :
	m_map(&map)
{

}

ScopedNavigabilityAlteration::~ScopedNavigabilityAlteration()
{
	for (const InitialTileNavigability& initialTileNavigability : m_initialTilesNavigability)
	{
		m_map->setTileNavigability(initialTileNavigability.tileIndex, initialTileNavigability.initialNavigability);
	}
}

void ScopedNavigabilityAlteration::setTileNavigability(map::TileIndex tileIndex, map::Navigability navigability)
{
	const map::Navigability initialTileNavigability = m_map->getTileNavigability(tileIndex);
	if (initialTileNavigability != navigability)
	{
		m_map->setTileNavigability(tileIndex, navigability);
		m_initialTilesNavigability.add(tileIndex, initialTileNavigability);
	}
}

} // map
} // game