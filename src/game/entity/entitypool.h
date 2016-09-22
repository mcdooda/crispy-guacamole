#ifndef GAME_ENTITY_ENTITYPOOL_H
#define GAME_ENTITY_ENTITYPOOL_H

#include <flat.h>
#include "entity.h"
#include "component/behaviorcomponent.h"
#include "component/movementcomponent.h"
#include "component/spritecomponent.h"

namespace game
{
namespace entity
{

class EntityPool
{
public:
	EntityPool() {}
	~EntityPool() {}

	Entity* createEntity(const std::shared_ptr<const EntityTemplate>& entityTemplate);
	void destroyEntity(Entity* entity);

	// create components
	template <class ComponentType>
	ComponentType* createComponent()
	{
		static_assert(std::is_base_of<component::Component, ComponentType>::value, "ComponentType must inherit from Component")
		FLAT_ASSERT(false);
	}

	template <>
	component::BehaviorComponent* createComponent<component::BehaviorComponent>()
	{
		return m_behaviorComponentPool.create();
	}

	template <>
	component::MovementComponent* createComponent<component::MovementComponent>()
	{
		return m_movementComponentPool.create();
	}

	template <>
	component::SpriteComponent* createComponent<component::SpriteComponent>()
	{
		return m_spriteComponentPool.create();
	}

	// destroy components
	template <class ComponentType>
	void destroyComponent(ComponentType* component)
	{
		FLAT_ASSERT(false);
	}

	template <>
	void destroyComponent(component::Component* component)
	{
		component::ComponentFlags type = component->getType();
		destroyComponentIfHasFlag<component::BehaviorComponent>(component, type);
		destroyComponentIfHasFlag<component::MovementComponent>(component, type);
		destroyComponentIfHasFlag<component::SpriteComponent>(component, type);
	}

	template <>
	void destroyComponent(component::BehaviorComponent* component)
	{
		m_behaviorComponentPool.destroy(component);
	}

	template <>
	void destroyComponent(component::MovementComponent* component)
	{
		m_movementComponentPool.destroy(component);
	}

	template <>
	void destroyComponent(component::SpriteComponent* component)
	{
		m_spriteComponentPool.destroy(component);
	}

	template <class ComponentType>
	void addComponentIfHasFlag(Entity* entity, component::ComponentFlags flags)
	{
		FLAT_ASSERT(entity != nullptr);
		if (component::hasFlag<ComponentType>(flags))
		{
			ComponentType* component = createComponent<ComponentType>();
			entity->addComponent(component);
		}
	}

	template <class ComponentType>
	void destroyComponentIfHasFlag(component::Component* component, component::ComponentFlags flags)
	{
		FLAT_ASSERT(component != nullptr);
		if (component::hasFlag<ComponentType>(flags))
		{
			FLAT_ASSERT(dynamic_cast<ComponentType*>(component) != nullptr);
			destroyComponent<ComponentType>(static_cast<ComponentType*>(component));
		}
	}

private:
	static const int MAX_ENTITIES = 5000;

	flat::containers::Pool<Entity, MAX_ENTITIES> m_entityPool;
	flat::containers::Pool<component::BehaviorComponent, MAX_ENTITIES> m_behaviorComponentPool;
	flat::containers::Pool<component::MovementComponent, MAX_ENTITIES> m_movementComponentPool;
	flat::containers::Pool<component::SpriteComponent, MAX_ENTITIES>   m_spriteComponentPool;
};

}
}

#endif // GAME_ENTITY_ENTITYPOOL_H


