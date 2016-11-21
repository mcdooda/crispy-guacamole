#ifndef GAME_ENTITY_ENTITYID_H
#define GAME_ENTITY_ENTITYID_H

#include <cstdint>

namespace game
{
namespace entity
{

using EntityId = uint64_t;

class EntityIdFactory
{
	static const EntityId firstTransientId = 1ull << 63;
	public:
		EntityIdFactory();

		EntityId makeTransientId();
		EntityId makeConstantId();

		inline static bool isTransientId(EntityId id) { return (id & firstTransientId) != 0; }
		inline bool isValidTransientId(EntityId id) { return firstTransientId <= id && id < m_nextTransientId; }

	public:
		static const EntityId InvalidId = 0;

	private:
		EntityId m_nextTransientId;
};

} // entity
} // game

#endif // GAME_ENTITY_ENTITYID_H


