#include <flat.h>
#include "entityid.h"

namespace game
{
namespace entity
{

EntityIdFactory::EntityIdFactory() :
	m_nextTransientId(firstTransientId)
{
}

EntityId EntityIdFactory::makeTransientId()
{
	EntityId id = m_nextTransientId;
	++m_nextTransientId;
	FLAT_ASSERT(isTransientId(id) && isValidTransientId(id));
	return id;
}

EntityId EntityIdFactory::makeConstantId()
{
	FLAT_ASSERT_MSG(false, "not implemented");
	EntityId id = 0;
	FLAT_ASSERT(!isTransientId(id));
	return id;
}

} // entity
} // game


