#ifndef GAME_ENTITY_COMPONENT_MOVEMENTCOMPONENT_H
#define GAME_ENTITY_COMPONENT_MOVEMENTCOMPONENT_H

#include <queue>
#include <flat.h>
#include "component.h"

namespace game
{
namespace map
{
class Tile;
}
namespace entity
{
namespace component
{

class MovementComponent : public Component
{
	typedef Component Super;
	public:
		void update(float currentTime, float elapsedTime) override;
		
		bool isBusy() const override;
		
		bool followsPath() const;
		void addPointOnPath(const flat::geometry::Vector2& point);
		
		struct Node
		{
			const map::Tile* tile;
			float distance;
			float heuristic;
			bool operator<(const Node& other) const { return heuristic < other.heuristic; }
			bool operator==(const Node& other) const { return tile == other.tile; }
		};
		
	private:
		void separateFromAdjacentTiles();
		void separateFromNearbyEntities();
		
		bool findPath(const flat::geometry::Vector2& from, const flat::geometry::Vector2& to, std::vector<flat::geometry::Vector2>& path) const;
		void reconstructPath(
			const std::map<const map::Tile*, const map::Tile*>& previous,
			const map::Tile* last,
			const flat::geometry::Vector2& from,
			const flat::geometry::Vector2& to,
			std::vector<flat::geometry::Vector2>& path) const;
		void simplifyPath(std::vector<flat::geometry::Vector2>& path) const;
		bool isStraightPath(const flat::geometry::Vector2& from, const flat::geometry::Vector2& to) const;
		
	private:
		std::queue<flat::geometry::Vector2> m_path;
};

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_MOVEMENTCOMPONENT_H



