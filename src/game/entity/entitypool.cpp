#include "entitypool.h"
#include "entitytemplate.h"

namespace game
{
namespace entity
{

EntityPool::EntityPool()
{
	size_t numComponentTypes = m_componentRegistry.getNumComponentTypes();
	m_componentPools.reserve(numComponentTypes);
	m_componentRegistry.eachComponentType([this](const component::ComponentType* componentType)
	{
		flat::containers::DynamicPool* componentPool = new flat::containers::DynamicPool();
		componentPool->init(componentType->getComponentSize(), MAX_ENTITIES);
		m_componentPools.emplace_back(componentPool);
	});
}

EntityPool::~EntityPool()
{

}

Entity* EntityPool::createEntity(const std::shared_ptr<const EntityTemplate>& entityTemplate, component::ComponentFlags componentsFilter)
{
	Entity* entity = m_entityPool.create(entityTemplate);
	component::ComponentFlags flags = entityTemplate.get()->getComponentFlags() & componentsFilter;

	m_componentRegistry.eachComponentType([this, entity, flags](const component::ComponentType* componentType)
	{
		flat::containers::DynamicPool* componentPool = m_componentPools[componentType->getComponentTypeId() - 1].get();
		const component::ComponentFlags componentTypeFlag = componentType->getComponentTypeFlag();
		if ((flags & componentTypeFlag) == componentTypeFlag)
		{
			component::Component* component = createComponent(componentType);
			entity->addComponent(component);
		}
	});

	entity->cacheComponents();
	entity->initComponents();
	return entity;
}

void EntityPool::destroyEntity(Entity* entity)
{
	FLAT_ASSERT(entity != nullptr);
	
	for (component::Component* component : entity->getComponents())
	{
		destroyComponent(component);
	}

	entity->removeAllComponents();
	m_entityPool.destroy(entity);
}

component::Component* EntityPool::createComponent(const component::ComponentType* componentType)
{
	flat::containers::DynamicPool* componentPool = getComponentPool(componentType);
	return componentType->createComponent(componentPool);
}

void EntityPool::destroyComponent(component::Component* component)
{
	const component::ComponentType* componentType = component->getComponentType();
	flat::containers::DynamicPool* componentPool = getComponentPool(componentType);
	componentType->destroyComponent(componentPool, component);
}

flat::containers::DynamicPool* EntityPool::getComponentPool(const component::ComponentType * componentType) const
{
	return m_componentPools[componentType->getComponentTypeId() - 1].get();
}

} // entity
} // game


