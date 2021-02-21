#ifndef GAME_ENTITY_ENTITYHELPER_H
#define GAME_ENTITY_ENTITYHELPER_H

#include <flat.h>

#include "../map/navigability.h"
#include "../map/tile.h"

namespace game
{
namespace entity
{
class Entity;
class EntityTemplate;

class EntityHelper
{
	public:
		static float getRadius(const Entity* entity);
		static flat::Vector3 getCollisionCenter(Entity* entity);
		static flat::Vector3 getBaseCenter(const Entity* entity);
		static void eachEntityTile(const Entity* entity, std::function<void(map::TileIndex)> func);

		static bool canCollide(const Entity* a, const Entity* b);

		static float getDistanceBetweenEntitiesWithRadius(const Entity* a, const Entity* b);

		static map::Navigability getNavigabilityMask(const Entity* entity);
		static map::Navigability getNavigabilityMask(const EntityTemplate* entityTemplate);

		static void setColorOverride(Entity* entity, const flat::video::Color& color);
		static void clearColorOverride(Entity* entity);
		static void setSelectedColor(Entity* entity);
		static void clearSelectedColor(Entity* entity);
		static void setMouseOverColor(Entity* entity);
		static void clearMouseOverColor(Entity* entity);

		static void mouseEntered(Entity* entity);
		static void mouseLeft(Entity* entity);
};

} // entity
} // game

#endif // GAME_ENTITY_ENTITYHELPER_H


