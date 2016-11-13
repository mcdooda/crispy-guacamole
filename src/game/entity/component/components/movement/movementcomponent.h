#ifndef GAME_ENTITY_COMPONENT_MOVEMENT_MOVEMENTCOMPONENT_H
#define GAME_ENTITY_COMPONENT_MOVEMENT_MOVEMENTCOMPONENT_H

#include <deque>
#include <flat.h>
#include "movementcomponenttemplate.h"
#include "../../component.h"

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
namespace movement
{

class MovementComponent : public ComponentImpl<MovementComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "movement"; }

		void init() override;

		void update(float currentTime, float elapsedTime) override;
		
		void addedToMap(map::Map* map);
		
		bool isBusy() const override;
		
		bool followsPath() const;
		void addPointOnPath(const flat::Vector2& point);
		
		void jump();
		inline bool isTouchingGround() const { return m_isTouchingGround; }

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	public:
		flat::Slot<> movementStarted;
		flat::Slot<> movementStopped;
		
	private:
		void fall(float elapsedTime);
		
	private:
		static constexpr float MIN_Z_EPSILON = 0.1f;
		
		std::deque<flat::Vector2> m_path; // TODO: use the right container like a traversable queue
		float m_zSpeed;
		bool m_isTouchingGround : 1;
};

} // movement
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_MOVEMENT_MOVEMENTCOMPONENT_H


