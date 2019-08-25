#ifndef GAME_MAP_SCOPEDNAVIGABILITYALTERATION_H
#define GAME_MAP_SCOPEDNAVIGABILITYALTERATION_H

#include <flat.h>

#include "tile.h"
#include "navigability.h"

namespace game
{
namespace map
{
class Map;

class ScopedNavigabilityAlteration
{
	private:
		struct InitialTileNavigability
		{
			InitialTileNavigability(map::TileIndex tileIndex, map::Navigability initialNavigability) :
				tileIndex(tileIndex),
				initialNavigability(initialNavigability)
			{}
			map::TileIndex tileIndex;
			map::Navigability initialNavigability;
		};

	public:
		ScopedNavigabilityAlteration() = delete;
		ScopedNavigabilityAlteration(Map& map);
		ScopedNavigabilityAlteration(const ScopedNavigabilityAlteration&) = delete;
		ScopedNavigabilityAlteration(ScopedNavigabilityAlteration&&) = delete;
		~ScopedNavigabilityAlteration();

		ScopedNavigabilityAlteration& operator=(const ScopedNavigabilityAlteration&) = delete;
		ScopedNavigabilityAlteration& operator=(ScopedNavigabilityAlteration&&) = delete;

		void setTileNavigability(map::TileIndex tileIndex, map::Navigability navigability);

	private:
		flat::containers::HybridArray<InitialTileNavigability, 4> m_initialTilesNavigability;
		map::Map* m_map;
};

} // game
} // map

#endif // GAME_MAP_SCOPEDNAVIGABILITYALTERATION_H