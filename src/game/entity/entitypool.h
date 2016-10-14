#ifndef GAME_ENTITY_ENTITYPOOL_H
#define GAME_ENTITY_ENTITYPOOL_H

#include <flat.h>
#include "entity.h"
#include "component/behaviorcomponent.h"
#include "component/movementcomponent.h"
#include "component/spritecomponent.h"
#include "component/componentregistry.h"

namespace game
{
namespace entity
{

class EntityPool
{
	public:
		EntityPool();
		~EntityPool();

		Entity* createEntity(
			const std::shared_ptr<const EntityTemplate>& entityTemplate,
			component::ComponentFlags componentsFilter = component::AllComponents
		);
		void destroyEntity(Entity* entity);

	private:
		component::Component* createComponent(const component::ComponentType* componentType);
		void destroyComponent(component::Component* component);

		flat::containers::DynamicPool* getComponentPool(const component::ComponentType* componentType) const;

	private:
		static const int MAX_ENTITIES = 5000;

		flat::containers::Pool<Entity, MAX_ENTITIES> m_entityPool;
		std::vector<std::unique_ptr<flat::containers::DynamicPool>> m_componentPools;
		flat::containers::Pool<component::BehaviorComponent, MAX_ENTITIES> m_behaviorComponentPool;
		flat::containers::Pool<component::MovementComponent, MAX_ENTITIES> m_movementComponentPool;
		flat::containers::Pool<component::SpriteComponent, MAX_ENTITIES>   m_spriteComponentPool;
		component::ComponentRegistry m_componentRegistry;
};

}
}

#endif // GAME_ENTITY_ENTITYPOOL_H


