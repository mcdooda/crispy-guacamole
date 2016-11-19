#include "entityhandle.h"
#include "entity.h"

namespace game
{
namespace entity
{

EntityHandle::EntityHandle() :
	m_entity(nullptr),
	m_id(EntityIdFactory::InvalidId)
{
}

EntityHandle::EntityHandle(EntityHandle& handle) :
	m_entity(handle.m_entity),
	m_id(handle.m_id)
{
	
}

EntityHandle::EntityHandle(Entity* entity) :
	m_entity(entity),
	m_id(entity->getId())
{
	FLAT_ASSERT(isValid());
}

bool EntityHandle::isValid() const
{
	return m_entity != nullptr && m_id != EntityIdFactory::InvalidId && m_entity->getId() == m_id;
}

Entity* EntityHandle::getEntity() const
{
	return isValid() ? m_entity : nullptr;
}

bool EntityHandle::operator==(const EntityHandle& other) const
{
	FLAT_ASSERT(m_id != other.m_id || m_entity == other.m_entity);
	return m_id == other.m_id;
}



} // entity
} // game


