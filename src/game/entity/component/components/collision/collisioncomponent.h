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

		void update(lua_State* L, float currentTime, float elapsedTime) override;

		void getAABB(flat::AABB3& aabb) const;

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	public:
		flat::Slot<lua_State*, Entity*> onCollidedWithEntity;
		flat::Slot<lua_State*> onCollidedWithMap;

	private:
		void separateFromNearbyEntities(lua_State* L);
		void separateFromAdjacentTiles(lua_State* L);

	private:
		static constexpr float MIN_Z_EPSILON = 0.1f;
};

} // collision
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_COLLISION_COLLISIONCOMPONENT_H


