#ifndef GAME_ENTITY_ENTITYUPDATER_H
#define GAME_ENTITY_ENTITYUPDATER_H

#include <vector>

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

#ifdef FLAT_DEBUG
		void debugDraw(debug::DebugDisplay& debugDisplay) const;
#endif

	private:
		const component::ComponentRegistry& m_componentRegistry;
		std::vector<ComponentBucketList> m_registeredComponents;
		std::vector<Entity*> m_registeredEntities;
		int m_updateIndex;

};

}
}

#endif // GAME_ENTITY_ENTITYUPDATER_H


