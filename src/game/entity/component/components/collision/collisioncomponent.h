#ifndef GAME_ENTITY_COMPONENT_COLLISIONCOMPONENT_H
#define GAME_ENTITY_COMPONENT_COLLISIONCOMPONENT_H

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

class CollisionComponent : public ComponentImpl<CollisionComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "collision"; }

		void update(float currentTime, float elapsedTime) override;

	public:
		flat::Slot<entity::Entity*> onCollidedWithEntity;
		flat::Slot<> onCollidedWithMap;

	private:
		void separateFromAdjacentTiles();
		void separateFromNearbyEntities();

	private:
		static constexpr float MIN_Z_EPSILON = 0.1f;
};

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_COLLISIONCOMPONENT_H



