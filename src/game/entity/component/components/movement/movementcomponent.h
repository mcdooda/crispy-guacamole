#ifndef GAME_ENTITY_COMPONENT_MOVEMENT_MOVEMENTCOMPONENT_H
#define GAME_ENTITY_COMPONENT_MOVEMENT_MOVEMENTCOMPONENT_H

#include <flat.h>

#include "movementcomponenttemplate.h"

#include "entity/component/component.h"

namespace game::map
{
class Map;
class Tile;
class Zone;
}

class game::entity::EntityUpdater;

namespace game::entity::component::movement
{

class MovementComponent : public ComponentImpl<MovementComponentTemplate>
{
	private:
		static constexpr int INVALID_POINT_INDEX = -1;
		static constexpr float MIN_DISTANCE_TO_DESTINATION = FLT_EPSILON;
		static constexpr float MAX_AVOIDANCE_RADIUS = 1.f;
		static constexpr float MIN_HEADING_CHANGE = flat::PI_2 / 64.f;
		static constexpr float JUMP_MIN_Z_DIFFERENCE = 0.1f;

	public:
		inline static const char* getConfigName() { return "movement"; }
		inline static const char* getVisualName() { return "Movement"; }

		void init() override;
		void deinit() override;

		void update(float currentTime, float elapsedTime) override;

		bool isBusy() const override;
		void cancelCurrentAction() override;

		void moveTo(const flat::Vector2& destination, Entity* interactionEntity = nullptr, bool allowPartialPath = true);

		void jump();

		inline bool isTouchingGround() const { return m_isTouchingGround; }

		inline void restrictToZone(const std::shared_ptr<const map::Zone>& zone) { m_restrictionZone = zone; }

		inline void setMovementSpeed(float movementSpeed) { FLAT_ASSERT(movementSpeed > 0.f); m_movementSpeed = movementSpeed; }
		inline float getMovementSpeed() const { return m_movementSpeed; }

		bool isFollowingPath() const;
		flat::Vector2 getCurrentMovementDirection() const;

		inline void setStrafing(bool isStrafing) { m_isStrafing = isStrafing; }
		inline bool isStrafing() const { return m_isStrafing; }

#ifdef FLAT_DEBUG
		void debugDraw(debug::DebugDisplay& debugDisplay) const override;
#endif

	public:
		flat::Slot<> movementStarted;
		flat::Slot<> movementStopped;

	private:
		const flat::Vector2& getNextPathPoint() const;
		void progressAlongPath(float elapsedTime);

		void avoidClosestEntity(flat::Vector2& steering) const;
		const Entity* getClosestEntityToAvoid(const flat::Vector2& steering, const flat::Matrix4& transform2dInverse) const;
		void getAvoidanceArea(flat::Vector2& center, float& radius) const;

		void startMovement();
		void stopMovement();

		void triggerStartStopCallbacks();

		void checkIsMidair();
		bool jumpIfNecessary(const flat::Vector2& steering);
		void fall(float elapsedTime);

		bool snapEntityToTile(Entity* entity, map::Map* map, EntityUpdater* entityUpdater);

		bool collidedWithMap(map::TileIndex tileIndex, const flat::Vector3& normal);

		void setPosition2d(const flat::Vector2& newPosition2d);

#ifdef FLAT_DEBUG
		void debugDrawCurrentPath(debug::DebugDisplay& debugDisplay) const;
		void debugDrawSteering(debug::DebugDisplay& debugDisplay) const;
		void debugDrawAvoidanceArea(debug::DebugDisplay& debugDisplay) const;
		void debugDrawEntity(debug::DebugDisplay& debugDisplay) const;

		void pathSanityCheck();
#endif

	private:
		// the path is a deque because the first point is removed once it's reached
		flat::sharp::ai::navigation::Path m_currentPath;
		int m_nextPathPointIndex;

		flat::Vector2 m_destination;
		entity::EntityHandle m_interactionEntity;

		// an entity pathfinding can be restricted to avoid getting outside of a given region (or zone)
		std::weak_ptr<const map::Zone> m_restrictionZone;

		// the 2d horizontal speed to use when moving
		float m_movementSpeed;

		// the current vertical speed, used when falling
		float m_currentVerticalSpeed;

		bool m_isTouchingGround : 1;

		// when strafing, the heading is not controlled by the movement component
		bool m_isStrafing : 1;

		bool m_wasMovingLastFrame : 1;

		bool m_isFollowingPartialPath : 1;

		bool m_hasWalkedOnTileCallback : 1;

#ifdef FLAT_DEBUG
		flat::Vector2 m_debugSteering;
#endif
};

} // game::entity::component::movement

#endif // GAME_ENTITY_COMPONENT_MOVEMENT_MOVEMENTCOMPONENT_H


