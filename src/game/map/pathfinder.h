#ifndef GAME_MAP_PATHFINDER_H
#define GAME_MAP_PATHFINDER_H

#include <flat.h>

namespace game
{
namespace map
{
class Map;
class Tile;

class Pathfinder
{
	public:
		struct Node
		{
			const map::Tile* tile;
			float distance;
			float heuristic;
			bool operator<(const Node& other) const { return heuristic < other.heuristic; }
			bool operator==(const Node& other) const { return tile == other.tile; }
		};
		
	public:
		Pathfinder() = delete;
		Pathfinder(const Map& map, float jumpHeight);
		
		bool findPath(const flat::Vector2& from, const flat::Vector2& to, std::vector<flat::Vector2>& path) const;
		
	private:
		void reconstructPath(
			const std::map<const map::Tile*, const map::Tile*>& previous,
			const map::Tile* last,
			const flat::Vector2& from,
			const flat::Vector2& to,
			std::vector<flat::Vector2>& path) const;
		void simplifyPath(std::vector<flat::Vector2>& path) const;
		bool isStraightPath(const flat::Vector2& from, const flat::Vector2& to) const;
		
	private:
		const Map& m_map;
		float m_jumpHeight;
};

} // map
} // game

#endif // GAME_MAP_PATHFINDER_H



