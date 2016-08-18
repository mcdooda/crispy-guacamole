#ifndef GAME_ENTITY_COMPONENT_MOVEMENTCOMPONENT_H
#define GAME_ENTITY_COMPONENT_MOVEMENTCOMPONENT_H

#include <queue>
#include <flat.h>
#include "component.h"

namespace game
{
namespace map
{
class Map;
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
		void setOwner(Entity* owner) override;
		void update(float currentTime, float elapsedTime) override;
		
		void addedToMap(map::Map* map);
		
		bool isBusy() const override;
		
		bool followsPath() const;
		void addPointOnPath(const flat::geometry::Vector2& point);
		
		void jump();
		inline bool isTouchingGround() const { return m_isTouchingGround; }
		
		struct Node
		{
			const map::Tile* tile;
			float distance;
			float heuristic;
			bool operator<(const Node& other) const { return heuristic < other.heuristic; }
			bool operator==(const Node& other) const { return tile == other.tile; }
		};
		
	private:
		void fall(float elapsedTime);
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
		static constexpr float MIN_Z_EPSILON = 0.1f;
		
		std::queue<flat::geometry::Vector2> m_path;
		float m_zSpeed;
		bool m_isTouchingGround : 1;
};

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_MOVEMENTCOMPONENT_H



