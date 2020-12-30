#ifndef GAME_MAP_PATHFINDER_H
#define GAME_MAP_PATHFINDER_H

#include <functional>
#include <flat.h>

#include "map/navigability.h"
#include "map/map.h"

namespace game::map::pathfinder
{

struct Request
{
	flat::Vector2 from;
	flat::Vector2 to;
	bool allowPartialResult = true;
	bool shouldOptimizePath = true;
};

class Pathfinder
{
	public:
		Pathfinder() = delete;
		Pathfinder(const Map& map, float jumpHeight, map::Navigability navigabilityMask);
		virtual ~Pathfinder() {}

		flat::sharp::ai::navigation::Result findPath(
			const Request& request,
			flat::sharp::ai::navigation::Path& path) const;

	protected:
		virtual void setupQuery(const Request& request, flat::sharp::ai::navigation::Query& query) const;

	protected:
		const Map& m_map;
		float m_jumpHeight;
		map::Navigability m_navigabilityMask;
};

} // game::map::pathfinder

#endif // GAME_MAP_PATHFINDER_H



