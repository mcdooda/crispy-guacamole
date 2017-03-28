#ifndef GAME_ENTITY_ENTITYHELPER_H
#define GAME_ENTITY_ENTITYHELPER_H

namespace game
{
namespace entity
{
class Entity;

class EntityHelper
{
	public:
		static float getRadius(const Entity* entity);
};

} // entity
} // game

#endif // GAME_ENTITY_ENTITYHELPER_H


