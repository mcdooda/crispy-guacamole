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
class Zone;
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
		void deinit() override;

		void update(float currentTime, float elapsedTime) override;
		
		bool isBusy() const override;
		void cancelCurrentAction() override;
		
		bool followsPath() const;
		flat::Vector2 getCurrentDirection() const;
		void moveTo(const flat::Vector2& point, Entity* interactionEntity = nullptr);
		void clearPath();
		
		void jump();
		inline bool isTouchingGround() const { return m_isTouchingGround; }

		inline void setMovementSpeed(float speed) { m_movementSpeed = speed; }
		inline float getMovementSpeed() const { return m_movementSpeed; }

		inline void setMidairAcceleration(const flat::Vector3& midairAcceleration) { m_midairAcceleration = midairAcceleration; }
		inline const flat::Vector3& getMidairAcceleration() const { return m_midairAcceleration; }

		inline bool isFollowingPath() const { return m_isFollowingPath; }

		inline void setIsStrafing(bool isStrafing) { m_isStrafing = isStrafing; }
		inline bool isStrafing() const { return m_isStrafing; }

		inline void restrictToZone(const std::shared_ptr<const map::Zone>& zone) { m_restrictToZone = zone; }

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	public:
		flat::Slot<> movementStarted;
		flat::Slot<> movementStopped;
		
	private:
		void fall(float elapsedTime);
		void land();

		bool addedToMap(Entity* entity, map::Map* map);
		bool removedFromMap(Entity* entity);

		bool collidedWithMap(map::TileIndex tileIndex, const flat::Vector3& normal);

		void updateSprite(bool movementStarted, bool movementStopped);
		bool updateSpritePosition(const flat::Vector3& position);
		
	private:
		static constexpr float MIN_Z_EPSILON = 0.1f;
		static constexpr float MIN_DISTANCE_TO_DESTINATION = FLT_EPSILON;
		static constexpr float RETURN_TO_DESTINATION_DURATION = 0.5f;
		
		std::deque<flat::Vector2> m_path; // TODO: use the right container like a traversable queue
		std::weak_ptr<const map::Zone> m_restrictToZone;
		flat::Vector2 m_destination;
		float m_returnToDestinationTime;
		float m_movementSpeed;
		flat::Vector3 m_midairVelocity;
		flat::Vector3 m_midairAcceleration;

		bool m_isTouchingGround : 1;
		bool m_isFollowingPath : 1;
		bool m_isStrafing : 1; // don't update the heading in this situation

		FLAT_DEBUG_ONLY(flat::Vector2 m_steering;)
};

} // movement
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_MOVEMENT_MOVEMENTCOMPONENT_H


