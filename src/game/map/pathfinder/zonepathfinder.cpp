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

const Tile* ZonePathfinder::getTileIfNavigable(float x, float y, map::Navigability navigabilityMask) const
{
	const Tile* tile = Super::getTileIfNavigable(x, y, navigabilityMask);
	if (m_zone->isTileInside(tile))
	{
		return tile;
	}
	return nullptr;
}

void ZonePathfinder::eachNeighborTiles(const Tile* tile, std::function<void(const Tile*)> func) const
{
	Super::eachNeighborTiles(tile, [this, &func](const Tile* tile)
	{
		if (m_zone->isTileInside(tile))
		{
			func(tile);
		}
	});
}

} // pathfinder
} // map
} // game


