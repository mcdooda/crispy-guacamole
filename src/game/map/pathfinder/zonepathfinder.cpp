#include "zonepathfinder.h"
#include "../zone.h"

namespace game::map::pathfinder
{

ZonePathfinder::ZonePathfinder(const Map& map, float jumpHeight, map::Navigability navigabilityMask, const Zone* zone) : Pathfinder(map, jumpHeight, navigabilityMask),
	m_zone(zone)
{
}

void ZonePathfinder::setupQuery(const Request& request, flat::sharp::ai::navigation::Query& query) const
{
	Super::setupQuery(request, query);

	query.areaFilter = [this](const flat::sharp::ai::navigation::NavigationData& navigationData, flat::sharp::ai::navigation::AreaId cellIndex) -> bool
	{
		return m_zone->isTileInside(static_cast<TileIndex>(cellIndex));
	};
}

} // game::map::pathfinder


