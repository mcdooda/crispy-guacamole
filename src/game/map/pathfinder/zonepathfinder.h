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
	typedef Pathfinder Super;
public:
	ZonePathfinder() = delete;
	ZonePathfinder(const Map& map, float jumpHeight, const Zone* zone);

protected:
	const Tile* getTileIfWalkable(float x, float y) const override;
	void eachNeighborTiles(const Tile* tile, std::function<void(const Tile*)> func) const override;

protected:
	const Zone* m_zone;
};

} // pathfinder
} // map
} // game

#endif // GAME_MAP_ZONEPATHFINDER_H



