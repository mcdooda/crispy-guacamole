#include <flat.h>
#include "entityupdater.h"

#include "component/componentregistry.h"

#include "component/components/life/lifecomponent.h"

namespace game
{
namespace entity
{

EntityUpdater::EntityUpdater(const component::ComponentRegistry& componentRegistry)
{
	size_t numComponentTypes = componentRegistry.getNumComponentTypes();
	m_registeredComponents.resize(numComponentTypes);
}

EntityUpdater::~EntityUpdater()
{
#ifdef FLAT_DEBUG
	FLAT_ASSERT(m_registeredEntities.size() == 0);

	for (const std::deque<component::Component*>& componentList : m_registeredComponents)
	{
		FLAT_ASSERT(componentList.size() == 0);
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
			m_registeredComponents[componentTypeId - 1].push_back(component);
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
			std::deque<component::Component*>& componentList = m_registeredComponents[componentTypeId - 1];
			std::deque<component::Component*>::iterator it = std::find(componentList.begin(), componentList.end(), component);
			FLAT_ASSERT(it != componentList.end());
			componentList.erase(it);
		}
	}
}

void EntityUpdater::unregisterAllEntities()
{
	m_registeredEntities.clear();

	for (std::deque<component::Component*>& componentList : m_registeredComponents)
	{
		componentList.clear();
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

	for (const std::deque<component::Component*>& componentList : m_registeredComponents)
	{
		if (componentList.size() > 0)
		{
			FLAT_PROFILE(componentList.front()->getComponentType().getConfigName());

			for (component::Component* component : componentList)
			{
				if (component->isEnabled())
				{
					component->update(time, dt);
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


