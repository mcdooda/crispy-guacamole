#include <set>
#include <algorithm>
#include "pathfinder.h"
#include "../map.h"
#include "../tile.h"

namespace game
{
namespace map
{
namespace pathfinder
{


Pathfinder::Pathfinder(const Map& map, float jumpHeight, map::Navigability navigabilityMask) :
	m_map(map),
	m_jumpHeight(jumpHeight),
	m_navigabilityMask(navigabilityMask)
{

}

flat::sharp::ai::navigation::Result Pathfinder::findPath(
	const Request& request,
	flat::sharp::ai::navigation::Path& path) const
{
	flat::sharp::ai::navigation::Query query;
	setupQuery(request, query);
	return m_map.getNavigationGrid().findPath(query, path);
}

void Pathfinder::setupQuery(const Request& request, flat::sharp::ai::navigation::Query& query) const
{
	query.from = request.from;
	query.to = request.to;
	query.allowPartialResult = request.allowPartialResult;
	query.shouldOptimizePath = request.shouldOptimizePath;
	query.jumpHeight = m_jumpHeight;
	query.navigabilityMask = m_navigabilityMask;
}

} // pathfinder
} // map
} // game



