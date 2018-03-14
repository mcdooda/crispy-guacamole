#ifndef GAME_ENTITY_ENTITY_H
#define GAME_ENTITY_ENTITY_H

#include <memory>
#include "entityhandle.h"
#include "../map/mapobject.h"
#include "component/componenttype.h"
#include "../debug/debugdisplay.h"

namespace game
{
class Game;
namespace map
{
class Map;
class Tile;
}
namespace entity
{
class EntityTemplate;
namespace component
{
class Component;
namespace behavior  { class BehaviorComponent; }
namespace collision { class CollisionComponent; }
namespace movement  { class MovementComponent; }
}

class Entity final : public map::MapObject
{
	public:
		Entity() = delete;
		Entity(const Entity&) = delete;
		Entity(Entity&&) = delete;
		Entity(const std::shared_ptr<const EntityTemplate>& entityTemplate, EntityId id);
		~Entity() override;
		void operator=(const Entity&) = delete;

		bool isEntity() const override { return true; }
		
		inline const std::shared_ptr<const EntityTemplate>& getEntityTemplate() const { return m_template; }

		inline EntityId getId() const { return m_id; }
		inline EntityHandle getHandle() const { return EntityHandle(this); }
		
		void setPosition(const flat::Vector3& position);
		void setXY(const flat::Vector2& xy);
		void setZ(float z);
		inline const flat::Vector3& getPosition() const { return m_position; }
		
		inline const map::Map* getMap() const { return m_map; }
		inline map::Map* getMap() { return m_map; }
		inline const map::Tile* getTile() const { return m_tile; }
		inline map::Tile* getTile() { return m_tile; }
		
		void setHeading(float heading);
		inline float getHeading() const { return m_heading; }

		void setElevation(float elevation);
		inline float getElevation() const { return m_elevation; }
		
		inline void setSprite(flat::render::Sprite* sprite) { FLAT_ASSERT(sprite != nullptr); m_sprite = sprite; }
		inline void clearSprite() { m_sprite = nullptr; }
		inline bool hasSprite() const { return m_sprite != nullptr; }
		flat::render::Sprite& getSprite() override;
		const flat::render::ProgramSettings& getProgramSettings() const override;
		
		void onAddedToMap(map::Map* map);
		void onRemovedFromMap();

#ifdef FLAT_DEBUG
		void debugDraw(debug::DebugDisplay& debugDisplay) const;
#endif
		
		bool isBusy() const;
		bool isBusy(component::ComponentFlags componentFlags) const;
		
		bool acceptsMoveOrders() const;
		void addPointOnPath(const flat::Vector2& point);
		void clearPath();
		
		void enterState(const char* stateName);

		void addComponent(component::Component* component);
		void cacheComponents();
		void initComponents();
		void deinitComponents();
		inline void resetComponents() { deinitComponents(); initComponents(); }

		template <class ComponentType>
		inline const ComponentType* getComponent() const;
		template <class ComponentType>
		inline ComponentType* getComponent();

		const component::Component* findComponent(component::ComponentFlags componentFlag) const;
		component::Component* findComponent(component::ComponentFlags componentFlag);

		inline const std::vector<component::Component*>& getComponents() const { return m_components; }
		inline void removeAllComponents() { m_components.clear(); }

		inline flat::lua::UniqueLuaReference<LUA_TTABLE>& getExtraData() { return m_extraData; }

		inline const std::shared_ptr<const EntityTemplate>& getTemplate() const { return m_template; }
		const std::string& getTemplateName() const;


		void setSelected(bool selected);
		inline bool isSelected() const { return m_selected; }
		inline void setCanBeSelected(bool canBeSelected) { m_canBeSelected = canBeSelected; }
		inline bool canBeSelected() const { return m_canBeSelected; }

		inline void markForDelete() { m_markedForDelete = true; }
		inline bool isMarkedForDelete() const { return m_markedForDelete; }

#ifdef FLAT_DEBUG
		inline void setDebug(bool debug) { m_debug = debug; }
		inline bool getDebug() const { return m_debug; }

		void setDebugAllComponents(bool debug);
		component::ComponentFlags getDebuggedComponentFlags() const;
		void setDebuggedComponentFlags(component::ComponentFlags debuggedComponentFlags);
#endif

		inline static void setEntityProgramSettings(const flat::render::ProgramSettings& programSettings)
		{
			Entity::entityProgramSettings = &programSettings;
		}

		inline static const flat::render::ProgramSettings& getEntityProgramSettings()
		{
			FLAT_ASSERT(entityProgramSettings != nullptr);
			return *entityProgramSettings;
		}

		inline void setAABBCanChange(bool aabbCanChange) { m_aabbCanChange = aabbCanChange; }
		void updateAABBIfDirty();
		
	public:
		flat::Slot<const flat::Vector3&> positionChanged;
		flat::Slot<float> headingChanged;
		flat::Slot<float> elevationChanged;
		flat::Slot<Entity*, map::Map*> addedToMap;
		flat::Slot<Entity*> removedFromMap;
		
	protected:
		map::Tile* getTileFromPosition();

		template <class ComponentType>
		inline const ComponentType* findComponent() const;
		template <class ComponentType>
		inline ComponentType* findComponent();
		
	protected:
		static const flat::render::ProgramSettings* entityProgramSettings;

		std::vector<component::Component*> m_components;
		component::behavior::BehaviorComponent*   m_behaviorComponent;
		component::collision::CollisionComponent* m_collisionComponent;
		component::movement::MovementComponent*   m_movementComponent;

		flat::render::Sprite* m_sprite;
		
		EntityId m_id;

		flat::Vector3 m_position;
		float m_heading;
		float m_elevation;

		flat::lua::UniqueLuaReference<LUA_TTABLE> m_extraData;
		
		map::Map* m_map;
		map::Tile* m_tile;
		
		std::shared_ptr<const EntityTemplate> m_template;

		bool m_canBeSelected : 1;
		bool m_selected : 1;
		bool m_markedForDelete : 1;
		bool m_aabbDirty : 1;
		bool m_aabbCanChange : 1;
#ifdef FLAT_DEBUG
		bool m_debug : 1;
#endif
};

template <class ComponentType>
inline const ComponentType* Entity::findComponent() const
{
	static_assert(std::is_base_of<component::Component, ComponentType>::value, "ComponentType must inherit from Component");
	const component::Component* component = findComponent(ComponentType::getFlag());
	FLAT_ASSERT(component == nullptr || dynamic_cast<const ComponentType*>(component) != nullptr);
	return static_cast<const ComponentType*>(component);
}

template <class ComponentType>
inline ComponentType* Entity::findComponent()
{
	static_assert(std::is_base_of<component::Component, ComponentType>::value, "ComponentType must inherit from Component");
	component::Component* component = findComponent(ComponentType::getFlag());
	FLAT_ASSERT(component == nullptr || dynamic_cast<ComponentType*>(component) != nullptr);
	return static_cast<ComponentType*>(component);
}

template <class ComponentType>
inline const ComponentType* Entity::getComponent() const
{
	return findComponent<ComponentType>();
}

template <class ComponentType>
inline ComponentType* Entity::getComponent()
{
	return findComponent<ComponentType>();
}

template <>
inline const component::behavior::BehaviorComponent* Entity::getComponent() const
{
	return m_behaviorComponent;
}
template <>
inline component::behavior::BehaviorComponent* Entity::getComponent()
{
	return m_behaviorComponent;
}

template <>
inline const component::collision::CollisionComponent* Entity::getComponent() const
{
	return m_collisionComponent;
}
template <>
inline component::collision::CollisionComponent* Entity::getComponent()
{
	return m_collisionComponent;
}

template <>
inline const component::movement::MovementComponent* Entity::getComponent() const
{
	return m_movementComponent;
}
template <>
inline component::movement::MovementComponent* Entity::getComponent()
{
	return m_movementComponent;
}

} // entity
} // game

#endif // GAME_ENTITY_ENTITY_H



