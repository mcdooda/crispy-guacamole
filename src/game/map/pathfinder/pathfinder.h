#ifndef GAME_MAP_PATHFINDER_H
#define GAME_MAP_PATHFINDER_H

#include <functional>
#include <flat.h>

#include "../navigability.h"
#include "../map.h"
#include "path.h"

namespace game
{
namespace map
{
namespace pathfinder
{

static constexpr int DEFAULT_ITERATION_LIMIT = 1500;

struct Request
{
	flat::Vector2 from;
	flat::Vector2 to;
	bool allowPartialResult = true;
};

enum class Result
{
	SUCCESS,
	PARTIAL,
	FAILURE
};

class Pathfinder
{
protected:
	struct Node
	{
		map::TileIndex tileIndex;
		float distance;
		float heuristic;
		bool operator<(const Node& other) const { return heuristic < other.heuristic; }
		bool operator==(const Node& other) const { return tileIndex == other.tileIndex; }
	};

public:
	Pathfinder() = delete;
	Pathfinder(const Map& map, float jumpHeight, map::Navigability navigabilityMask);
	virtual ~Pathfinder() {}

	Result findPath(const Request& request, Path& path) const;

protected:
	virtual TileIndex getTileIndexIfNavigable(float x, float y, map::Navigability navigabilityMask) const;
	void reconstructPath(
		const std::map<TileIndex, TileIndex>& previous,
		const map::TileIndex lastIndex,
		const flat::Vector2& from,
		const flat::Vector2& to,
		Result pathfindingResult,
		std::vector<flat::Vector2>& path) const;
	virtual void eachNeighborTiles(TileIndex tile, std::function<void(TileIndex)> func) const;

private:
	Result findPath(const flat::Vector2& from, const flat::Vector2& to, bool allowPartialResults, std::vector<flat::Vector2>& path) const;

protected:
	const Map& m_map;
	float m_jumpHeight;
	map::Navigability m_navigabilityMask;
};

} // pathfinder
} // map
} // game

#endif // GAME_MAP_PATHFINDER_H



