#include <flat.h>
#include "entityupdater.h"

#include "component/componentregistry.h"

#include "component/components/life/lifecomponent.h"

namespace game
{
namespace entity
{

EntityUpdater::EntityUpdater(const component::ComponentRegistry& componentRegistry) :
	m_componentRegistry(componentRegistry),
	m_updateIndex(0)
{
	size_t numComponentTypes = componentRegistry.getNumComponentTypes();
	m_registeredComponents.resize(numComponentTypes);
	componentRegistry.eachComponentType(
		[this](component::ComponentType& componentType)
		{
			if (componentType.requiresUpdate())
			{
				FLAT_ASSERT(componentType.getUpdatePeriod() >= 1);
				m_registeredComponents[componentType.getComponentTypeId() - 1].resize(componentType.getUpdatePeriod());
			}
		}
	);
}

EntityUpdater::~EntityUpdater()
{
#ifdef FLAT_DEBUG
	FLAT_ASSERT(m_registeredEntities.size() == 0);

	for (const ComponentBucketList& componentBucketList : m_registeredComponents)
	{
		for (const ComponentBucket& bucket : componentBucketList)
		{
			FLAT_ASSERT(bucket.size() == 0);
		}
	}
#endif

	m_registeredComponents.clear();
}

void EntityUpdater::registerEntity(Entity* entity)
{
	FLAT_PROFILE("Register entity");

	FLAT_ASSERT(entity->getMap() == nullptr);

	FLAT_ASSERT(std::find(m_registeredEntities.begin(), m_registeredEntities.end(), entity) == m_registeredEntities.end());
	m_registeredEntities.push_back(entity);

	for (component::Component* component : entity->getComponents())
	{
		if (component->componentRequiresUpdate())
		{
			component::ComponentTypeId componentTypeId = component->getComponentType().getComponentTypeId();
			ComponentBucketList& componentBucketList = m_registeredComponents[componentTypeId - 1];
			ComponentBucket& bucket = componentBucketList[entity->getId() % component->getComponentUpdatePeriod()];
			bucket.push_back(component);
		}
	}
}

void EntityUpdater::unregisterEntity(Entity* entity)
{
	std::vector<Entity*>::iterator it = std::find(m_registeredEntities.begin(), m_registeredEntities.end(), entity);
	FLAT_ASSERT(it != m_registeredEntities.end());
	m_registeredEntities.erase(it);

	for (component::Component* component : entity->getComponents())
	{
		if (component->componentRequiresUpdate())
		{
			component::ComponentTypeId componentTypeId = component->getComponentType().getComponentTypeId();
			ComponentBucketList& componentBucketList = m_registeredComponents[componentTypeId - 1];
			ComponentBucket& bucket = componentBucketList[entity->getId() % component->getComponentUpdatePeriod()];
			ComponentBucket::iterator it = std::find(bucket.begin(), bucket.end(), component);
			FLAT_ASSERT(it != bucket.end());
			*it = bucket.back();
			bucket.pop_back();
		}
	}
}

void EntityUpdater::unregisterAllEntities()
{
	m_registeredEntities.clear();

	for (ComponentBucketList& componentBucketList : m_registeredComponents)
	{
		for (ComponentBucket& componentBucket : componentBucketList)
		{
			componentBucket.clear();
		}
	}
}

void EntityUpdater::updateSingleEntity(Entity* entity, float time, float dt)
{
	FLAT_PROFILE("Update single entity");

	FLAT_ASSERT(entity->getMap() != nullptr);

	for (component::Component* component : entity->getComponents())
	{
		if (component->componentRequiresUpdate() && component->isEnabled())
		{
			FLAT_PROFILE(component->getComponentType().getConfigName());
			component->update(time, dt);
		}
	}

	entity->updateAABBIfDirty();
}

void EntityUpdater::updateAllEntities(float time, float dt)
{
	FLAT_PROFILE("Update all entities");

	for (int i = 0; i < m_registeredComponents.size(); ++i)
	{
		const ComponentBucketList& componentBucketList = m_registeredComponents[i];
		const component::ComponentFlags componentFlag = 1 << (i + 1);
		const component::ComponentType& componentType = m_componentRegistry.getComponentType(componentFlag);
		if (componentType.requiresUpdate())
		{
			const int updatePeriod = componentType.getUpdatePeriod();
			FLAT_ASSERT(componentBucketList.size() > 0);
			const ComponentBucket& bucket = componentBucketList[m_updateIndex % updatePeriod];
			if (bucket.size() > 0)
			{
				FLAT_PROFILE(componentType.getConfigName());

				for (component::Component* component : bucket)
				{
					if (component->isEnabled())
					{
						component->update(time, dt);
					}
				}
			}
		}
	}

	{
		FLAT_PROFILE("Update AABBs");
		for (entity::Entity* entity : m_registeredEntities)
		{
			entity->updateAABBIfDirty();
		}
	}

	++m_updateIndex;
}

#ifdef FLAT_DEBUG
void EntityUpdater::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	for (entity::Entity* entity : m_registeredEntities)
	{
		entity->debugDraw(debugDisplay);
	}
}
#endif

} // entity
} // game


