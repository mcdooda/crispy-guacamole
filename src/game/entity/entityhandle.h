#ifndef GAME_ENTITY_ENTITYHANDLE_H
#define GAME_ENTITY_ENTITYHANDLE_H

#include "entityid.h"

namespace game
{
namespace entity
{
class Entity;

class EntityHandle
{
	public:
		EntityHandle();
		EntityHandle(EntityHandle& handle);
		EntityHandle(Entity* entity);

		bool isValid() const;
		Entity* getEntity() const;

		bool operator==(const EntityHandle& other) const;

	public:
		static const EntityHandle InvalidHandle;

	private:
		Entity* m_entity;
		EntityId m_id;
};

} // entity
} // game

#endif // GAME_ENTITY_ENTITYHANDLE_H


