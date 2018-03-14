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
	for (component::Component* component : entity->getComponents())
	{
		if (component->componentRequiresUpdate())
		{
			component->update(time, dt);
		}
	}
}

void EntityUpdater::updateAllEntities(float time, float dt)
{
	for (const std::deque<component::Component*>& componentList : m_registeredComponents)
	{
		for (component::Component* component : componentList)
		{
			component->update(time, dt);
		}
	}

	for (entity::Entity* entity : m_registeredEntities)
	{
		entity->updateAABBIfDirty();
#ifdef FLAT_DEBUG
		// ensure all AABBs are up to date
		if (entity->hasSprite())
		{
			flat::AABB2 spriteAABB;
			entity->getSprite().getAABB(spriteAABB);
			FLAT_ASSERT(spriteAABB == entity->getAABB());
		}
	}
#endif
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


