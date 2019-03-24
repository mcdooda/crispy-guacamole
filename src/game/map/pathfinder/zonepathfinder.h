#ifndef GAME_MAP_ZONEPATHFINDER_H
#define GAME_MAP_ZONEPATHFINDER_H

#include "pathfinder.h"

namespace game
{
namespace map
{
class Zone;
namespace pathfinder
{

class ZonePathfinder : public Pathfinder
{
	using Super = Pathfinder;
public:
	ZonePathfinder() = delete;
	ZonePathfinder(const Map& map, float jumpHeight, map::Navigability navigabilityMask, const Zone* zone);

protected:
	TileIndex getTileIndexIfNavigable(float x, float y, map::Navigability navigabilityMask) const override;
	void eachNeighborTiles(TileIndex tileIndex, std::function<void(TileIndex)> func) const override;

protected:
	const Zone* m_zone;
};

} // pathfinder
} // map
} // game

#endif // GAME_MAP_ZONEPATHFINDER_H



