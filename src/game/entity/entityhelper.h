#ifndef GAME_ENTITY_ENTITYHELPER_H
#define GAME_ENTITY_ENTITYHELPER_H

#include "../map/navigability.h"

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

		static float getDistanceBetweenEntitiesWithRadius(const Entity* a, const Entity* b);

		static map::Navigability getNavigabilityMask(const Entity* entity);
		static map::Navigability getNavigabilityMask(const EntityTemplate* entityTemplate);
};

} // entity
} // game

#endif // GAME_ENTITY_ENTITYHELPER_H


