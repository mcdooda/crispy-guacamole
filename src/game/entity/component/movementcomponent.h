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
		enum { Type = 1 << 1 };
	public:
		void setOwner(Entity* owner) override;
		void update(float currentTime, float elapsedTime) override;
		
		void addedToMap(map::Map* map);
		
		bool isBusy() const override;

		ComponentFlags getType() const override { return Type; }
		
		bool followsPath() const;
		void addPointOnPath(const flat::Vector2& point);
		
		void jump();
		inline bool isTouchingGround() const { return m_isTouchingGround; }
		
	private:
		void fall(float elapsedTime);
		void separateFromAdjacentTiles();
		void separateFromNearbyEntities();
		
	private:
		static constexpr float MIN_Z_EPSILON = 0.1f;
		
		std::queue<flat::Vector2> m_path;
		float m_zSpeed;
		bool m_isTouchingGround : 1;
};

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_MOVEMENTCOMPONENT_H



