#ifndef GAME_ENTITY_ENTITYPOOL_H
#define GAME_ENTITY_ENTITYPOOL_H

#include <flat.h>
#include "entity.h"
#include "component/component.h"

namespace game
{
namespace entity
{
namespace component
{
class ComponentRegistry;
}

class EntityPool
{
	public:
		EntityPool(const component::ComponentRegistry& componentRegistry);
		~EntityPool();

		Entity* createEntity(
			const std::shared_ptr<const EntityTemplate>& entityTemplate,
			const component::ComponentRegistry& componentRegistry,
			component::ComponentFlags componentsFilter = component::AllComponents
		);
		void destroyEntity(Entity* entity);

	private:
		component::Component* createComponent(const component::ComponentType& componentType);
		void destroyComponent(component::Component* component);

		flat::containers::DynamicPool& getComponentPool(const component::ComponentType& componentType) const;

	private:
		static const int MAX_ENTITIES = 5000;

		EntityIdFactory m_entityIdFactory;
		flat::containers::Pool<Entity, MAX_ENTITIES> m_entityPool;
		std::vector<std::unique_ptr<flat::containers::DynamicPool>> m_componentPools;

};

}
}

#endif // GAME_ENTITY_ENTITYPOOL_H


