#ifndef GAME_ENTITY_ENTITYUPDATER_H
#define GAME_ENTITY_ENTITYUPDATER_H

#include <deque>
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
		std::vector<std::deque<component::Component*>> m_registeredComponents;
		std::vector<Entity*> m_registeredEntities;

};

}
}

#endif // GAME_ENTITY_ENTITYUPDATER_H


