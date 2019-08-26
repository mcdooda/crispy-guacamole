#ifndef GAME_MAP_PATHFINDER_H
#define GAME_MAP_PATHFINDER_H

#include <functional>
#include <flat.h>

#include "../navigability.h"
#include "../map.h"

namespace game
{
namespace map
{
namespace pathfinder
{

class Pathfinder
{
public:
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

	bool findPath(const flat::Vector2& from, const flat::Vector2& to, std::vector<flat::Vector2>& path) const;

#ifdef FLAT_DEBUG
	static void enableSimplifyPath(bool shouldSimplifyPath) { Pathfinder::shouldSimplifyPath = shouldSimplifyPath; }
#endif

protected:
	virtual TileIndex getTileIndexIfNavigable(float x, float y, map::Navigability navigabilityMask) const;
	void reconstructPath(
		const std::map<TileIndex, TileIndex>& previous,
		const map::TileIndex lastIndex,
		const flat::Vector2& from,
		const flat::Vector2& to,
		std::vector<flat::Vector2>& path) const;
	void simplifyPath(std::vector<flat::Vector2>& path) const;
	bool isStraightPath(const flat::Vector2& from, const flat::Vector2& to) const;
	virtual void eachNeighborTiles(TileIndex tile, std::function<void(TileIndex)> func) const;

protected:
#ifdef FLAT_DEBUG
	static bool shouldSimplifyPath;
#endif

	const Map& m_map;
	float m_jumpHeight;
	map::Navigability m_navigabilityMask;
};

} // pathfinder
} // map
} // game

#endif // GAME_MAP_PATHFINDER_H



