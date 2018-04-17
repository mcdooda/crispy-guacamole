#include <future>
#include <thread>
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
	m_registeredComponents.reserve(numComponentTypes);
	componentRegistry.eachComponentType([this](const component::ComponentType& componentType)
	{
		m_registeredComponents.emplace_back(componentType);
	});
}

EntityUpdater::~EntityUpdater()
{
#ifdef FLAT_DEBUG
	FLAT_ASSERT(m_registeredEntities.size() == 0);

	for (const ComponentList& componentList : m_registeredComponents)
	{
		FLAT_ASSERT(componentList.components.size() == 0);
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
			m_registeredComponents[componentTypeId - 1].components.push_back(component);
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
			std::deque<component::Component*>& componentList = m_registeredComponents[componentTypeId - 1].components;
			std::deque<component::Component*>::iterator it = std::find(componentList.begin(), componentList.end(), component);
			FLAT_ASSERT(it != componentList.end());
			componentList.erase(it);
		}
	}
}

void EntityUpdater::unregisterAllEntities()
{
	m_registeredEntities.clear();

	for (ComponentList& componentList : m_registeredComponents)
	{
		componentList.components.clear();
	}
}

void EntityUpdater::updateSingleEntity(Entity* entity, float time, float dt) const
{
	for (component::Component* component : entity->getComponents())
	{
		if (component->componentRequiresUpdate() && component->isEnabled())
		{
			component->update(time, dt);
		}
	}

	entity->updateAABBIfDirty();
}

void EntityUpdater::updateAllEntities(float time, float dt) const
{
	for (const ComponentList& componentList : m_registeredComponents)
	{
		if (componentList.componentType.requiresSingleThreadedUpdate())
		{
			updateRange(componentList.components.begin(), componentList.components.end(), time, dt);
		}
		else
		{
			updateMultiThread(componentList, time, dt);
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

void EntityUpdater::updateRange(std::deque<component::Component*>::const_iterator begin, std::deque<component::Component*>::const_iterator end, float time, float dt) const
{
	for (std::deque<component::Component*>::const_iterator it = begin; it != end; ++it)
	{
		component::Component* component = *it;
		if (component->isEnabled())
		{
			component->update(time, dt);
		}
	}
}

void EntityUpdater::updateMultiThread(const ComponentList& componentList, float time, float dt) const
{
	const unsigned int numComponents = static_cast<unsigned int>(componentList.components.size());
	const unsigned int numThreads = std::min(8U, numComponents);

	const unsigned int numComponentsPerThread = static_cast<unsigned int>(std::ceil(static_cast<float>(numComponents) / numThreads));

	std::vector<std::future<void>> futures(numThreads);

	for (unsigned int i = 0; i < numThreads; ++i)
	{
		int beginIndex = numComponentsPerThread * i;
		int endIndex = std::min(beginIndex + numComponentsPerThread, numComponents);
		std::deque<component::Component*>::const_iterator begin = componentList.components.cbegin() + beginIndex;
		std::deque<component::Component*>::const_iterator end = componentList.components.cbegin() + endIndex;
		futures[i] = std::async(
			[this, begin, end, time, dt]()
			{
				updateRange(begin, end, time, dt);
			}
		);
	}

	for (std::future<void>& future : futures)
	{
		future.wait();
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


