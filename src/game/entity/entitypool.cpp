#include "entitypool.h"
#include "entitytemplate.h"
#include "component/componentregistry.h"

namespace game
{
namespace entity
{

EntityPool::EntityPool(const component::ComponentRegistry& componentRegistry)
{
	size_t numComponentTypes = componentRegistry.getNumComponentTypes();
	m_componentPools.reserve(numComponentTypes);
	componentRegistry.eachComponentType([this](const component::ComponentType& componentType)
	{
		flat::containers::DynamicPool* componentPool = new flat::containers::DynamicPool();
		componentPool->init(componentType.getComponentSize(), MAX_ENTITIES);
		m_componentPools.emplace_back(componentPool);
	});
}

EntityPool::~EntityPool()
{
	FLAT_ASSERT(m_entityPool.getNumAllocatedObjects() == 0);
}

Entity* EntityPool::createEntity(
	const std::shared_ptr<const EntityTemplate>& entityTemplate,
	const component::ComponentRegistry& componentRegistry,
	component::ComponentFlags componentsFilter
)
{
	Entity* entity = m_entityPool.create(entityTemplate, m_entityIdFactory.makeTransientId());
	component::ComponentFlags flags = entityTemplate.get()->getComponentFlags() & componentsFilter;

	componentRegistry.eachComponentType([this, entity, flags](const component::ComponentType& componentType)
	{
		const component::ComponentFlags componentTypeFlag = componentType.getComponentTypeFlag();
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
	
	entity->deinitComponents();
	for (component::Component* component : entity->getComponents())
	{
		destroyComponent(component);
	}

	entity->removeAllComponents();
	m_entityPool.destroy(entity);
}

component::Component* EntityPool::createComponent(const component::ComponentType& componentType)
{
	flat::containers::DynamicPool& componentPool = getComponentPool(componentType);
	return componentType.createComponent(componentPool);
}

void EntityPool::destroyComponent(component::Component* component)
{
	const component::ComponentType& componentType = component->getComponentType();
	flat::containers::DynamicPool& componentPool = getComponentPool(componentType);
	componentType.destroyComponent(componentPool, component);
}

flat::containers::DynamicPool& EntityPool::getComponentPool(const component::ComponentType& componentType) const
{
	return *m_componentPools[componentType.getComponentTypeId() - 1].get();
}

} // entity
} // game


