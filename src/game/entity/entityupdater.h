#ifndef GAME_ENTITY_ENTITYUPDATER_H
#define GAME_ENTITY_ENTITYUPDATER_H

#include <vector>

#include "entity/component/componenttype.h"

namespace game
{
#ifdef FLAT_DEBUG
namespace debug
{
class DebugDisplay;
}
#endif
namespace entity
{
class Entity;
namespace component
{
class Component;
class ComponentRegistry;
}

class ComponentPostCall
{
	public:
		ComponentPostCall(component::Component* component) : m_component(component) {}

		Entity* getEntity() const;

		virtual void operator()(float time, float dt) = 0;

	protected:
		component::Component* m_component;
};

template <typename T>
class ComponentPostCallImpl final : public ComponentPostCall
{
	public:
		using CallbackType = void (T::*)(float, float);

	public:
		ComponentPostCallImpl(T* component, CallbackType callbackMethod) : ComponentPostCall(component),
			m_callbackMethod(callbackMethod) {}

		void operator()(float time, float dt) override
		{
			FLAT_ASSERT(*reinterpret_cast<uint8_t*>(m_component) != FLAT_WIPE_VALUE);
			return (static_cast<T*>(m_component)->*m_callbackMethod)(time, dt);
		}

	private:
		CallbackType m_callbackMethod;
};

class EntityUpdater final
{
	private:
		using ComponentBucket = std::vector<component::Component*>;
		using ComponentBucketList = std::vector<ComponentBucket>;

	public:
		EntityUpdater() = delete;
		EntityUpdater(const EntityUpdater&) = delete;
		EntityUpdater(EntityUpdater&&) = delete;
		EntityUpdater(const component::ComponentRegistry& componentRegistry);
		~EntityUpdater();

		void operator=(const EntityUpdater&) = delete;

		void registerEntity(Entity* entity);
		void unregisterEntity(Entity* entity);
		void unregisterAllEntities();

		void updateSingleEntity(Entity* entity, float time, float dt);
		void updateAllEntities(float time, float dt);

		const std::vector<Entity*>& getEntities() const { return m_registeredEntities; }
		std::vector<Entity*>& getEntities() { return m_registeredEntities; }

		template <class T>
		void addComponentPostCall(T* component, void (T::* callbackMethod)(float, float));

		void triggerComponentPostCalls(Entity* entity, float time, float dt);

#ifdef FLAT_DEBUG
		void debugDraw(debug::DebugDisplay& debugDisplay) const;
#endif

	private:
		using ComponentUpdateMethod = void (component::Component::*)(float time, float dt);
		void updateSingleEntityComponents(Entity* entity, float time, float dt, component::ComponentUpdateType updateType, ComponentUpdateMethod updateMethod);
		void updateAllEntitiesComponents(float time, float dt, component::ComponentUpdateType updateType, ComponentUpdateMethod updateMethod);

	private:
		const component::ComponentRegistry& m_componentRegistry;
		std::vector<ComponentBucketList> m_registeredComponents;
		std::vector<Entity*> m_registeredEntities;
		std::vector<std::unique_ptr<ComponentPostCall>> m_componentPostCalls;
		int m_updateIndex;

};

template <class T>
void EntityUpdater::addComponentPostCall(T* component, void (T::* callbackMethod)(float, float))
{
	m_componentPostCalls.emplace_back(std::make_unique<ComponentPostCallImpl<T>>(component, callbackMethod));
}

}
}

#endif // GAME_ENTITY_ENTITYUPDATER_H


