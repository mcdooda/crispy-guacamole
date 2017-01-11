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

EntityHandle::EntityHandle(const EntityHandle& handle) :
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

void EntityHandle::operator=(EntityHandle& handle)
{
	m_entity = handle.m_entity;
	m_id = handle.m_id;
}

void EntityHandle::operator=(Entity* entity)
{
	m_entity = entity;
	m_id = entity != nullptr ? entity->getId() : EntityIdFactory::InvalidId;
}

bool EntityHandle::isValid() const
{
	return m_entity != nullptr && m_id != EntityIdFactory::InvalidId && m_entity->getId() == m_id;
}

} // entity
} // game


