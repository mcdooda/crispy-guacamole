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

		void operator=(EntityHandle& handle);
		void operator=(Entity* entity);

		bool isValid() const;
		inline Entity* getEntity() const { return isValid() ? m_entity : nullptr; }

		inline bool operator==(const EntityHandle& other) const { return m_id == other.m_id; }
		inline bool operator!=(const EntityHandle& other) const { return m_id != other.m_id; }

	public:
		static const EntityHandle InvalidHandle;

	private:
		Entity* m_entity;
		EntityId m_id;
};

} // entity
} // game

#endif // GAME_ENTITY_ENTITYHANDLE_H

