#include "entitypool.h"
#include "entitytemplate.h"

namespace game
{
namespace entity
{

Entity* EntityPool::createEntity(const std::shared_ptr<const EntityTemplate>& entityTemplate, component::ComponentFlags componentsFilter)
{
	Entity* entity = m_entityPool.create(entityTemplate);
	component::ComponentFlags flags = entityTemplate.get()->getComponentFlags() & componentsFilter;
	addComponentIfHasFlag<component::BehaviorComponent>(entity, flags);
	addComponentIfHasFlag<component::MovementComponent>(entity, flags);
	addComponentIfHasFlag<component::SpriteComponent>(entity, flags);
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

} // entity
} // game


