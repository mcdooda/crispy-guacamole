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
		inline static const char* getVisualName() { return "Collision"; }

		void update(float currentTime, float elapsedTime) override;

		void getAABB(flat::AABB3& aabb) const;
		float getBottom(float z) const;

		flat::Vector3 getCenter() const;

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	public:
		flat::Slot<Entity*, const flat::Vector3&> onCollidedWithEntity;
		flat::Slot<map::TileIndex, const flat::Vector3&> onCollidedWithMap;

	private:
		void separateFromNearbyEntities();
		void separateFromAdjacentTiles();
};

} // collision
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_COLLISION_COLLISIONCOMPONENT_H


