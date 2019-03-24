#ifndef GAME_ENTITY_COMPONENT_COLLISION_COLLISIONCOMPONENT_H
#define GAME_ENTITY_COMPONENT_COLLISION_COLLISIONCOMPONENT_H

#include <queue>
#include <flat.h>

#include "collisioncomponenttemplate.h"
#include "../../component.h"
#include "../../../../map/tile.h"

namespace game
{
namespace entity
{
namespace component
{
namespace collision
{

class CollisionComponent : public ComponentImpl<CollisionComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "collision"; }

		void update(float currentTime, float elapsedTime) override;

		void getAABB(flat::AABB3& aabb) const;
		float getBottom() const;

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	public:
		flat::Slot<Entity*, const flat::Vector3&> onCollidedWithEntity;
		flat::Slot<map::TileIndex, const flat::Vector3&> onCollidedWithMap;

	private:
		void separateFromNearbyEntities();
		void separateFromAdjacentTiles();

	private:
		static constexpr float MIN_Z_EPSILON = 0.1f;
};

} // collision
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_COLLISION_COLLISIONCOMPONENT_H


