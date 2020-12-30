#ifndef GAME_MAP_ZONEPATHFINDER_H
#define GAME_MAP_ZONEPATHFINDER_H

#include "pathfinder.h"

namespace game::map
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
		virtual void setupQuery(const Request& request, flat::sharp::ai::navigation::Query& query) const override;

protected:
		const Zone* m_zone;
};

} // pathfinder
} // game::map

#endif // GAME_MAP_ZONEPATHFINDER_H



