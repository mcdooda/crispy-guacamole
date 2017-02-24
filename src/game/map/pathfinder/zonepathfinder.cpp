#include "zonepathfinder.h"
#include "../zone.h"

namespace game
{
namespace map
{
namespace pathfinder
{

ZonePathfinder::ZonePathfinder(const Map& map, float jumpHeight, const Zone * zone) : Pathfinder(map, jumpHeight),
	m_zone(zone)
{
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


