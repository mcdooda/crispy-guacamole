#include "zonepathfinder.h"
#include "../zone.h"

namespace game
{
namespace map
{
namespace pathfinder
{

ZonePathfinder::ZonePathfinder(const Map& map, float jumpHeight, map::Navigability navigabilityMask, const Zone* zone) : Pathfinder(map, jumpHeight, navigabilityMask),
	m_zone(zone)
{
}

TileIndex ZonePathfinder::getTileIndexIfNavigable(float x, float y, map::Navigability navigabilityMask) const
{
	TileIndex tileIndex = Super::getTileIndexIfNavigable(x, y, navigabilityMask);
	if (m_zone->isTileInside(tileIndex))
	{
		return tileIndex;
	}
	return TileIndex::INVALID;
}

void ZonePathfinder::eachNeighborTiles(TileIndex tileIndex, std::function<void(TileIndex)> func) const
{
	Super::eachNeighborTiles(tileIndex, [this, &func](TileIndex neighborTileIndex)
	{
		if (m_zone->isTileInside(neighborTileIndex))
		{
			func(neighborTileIndex);
		}
	});
}

} // pathfinder
} // map
} // game


