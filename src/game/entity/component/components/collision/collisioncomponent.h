#ifndef GAME_ENTITY_COMPONENT_COLLISION_COLLISIONCOMPONENT_H
#define GAME_ENTITY_COMPONENT_COLLISION_COLLISIONCOMPONENT_H

#include <queue>
#include <flat.h>
#include "collisioncomponenttemplate.h"
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
namespace collision
{

class CollisionComponent : public ComponentImpl<CollisionComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "collision"; }

		void update(float currentTime, float elapsedTime) override;

		void getAABB(flat::AABB3& aabb) const;

	public:
		flat::Slot<Entity*> onCollidedWithEntity;
		flat::Slot<> onCollidedWithMap;

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


