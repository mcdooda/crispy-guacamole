#ifndef GAME_ENTITY_ENTITY_H
#define GAME_ENTITY_ENTITY_H

#include <memory>

#include "entityhandle.h"
#include "component/componenttype.h"
#include "../debug/debugdisplay.h"
#include "../map/mapobject.h"
#include "../map/tile.h"

namespace game
{
class Game;
namespace entity
{
class EntityTemplate;
class EntityUpdater;
namespace component
{
class Component;
namespace behavior  { class BehaviorComponent; }
namespace collision { class CollisionComponent; }
namespace movement  { class MovementComponent; }
namespace sprite    { class SpriteComponent; }
}

class Entity final : public map::MapObject
{
	public:
		static constexpr float MIN_Z_EPSILON = 0.1f;

	public:
		Entity() = delete;
		Entity(const Entity&) = delete;
		Entity(Entity&&) = delete;
		Entity(const std::shared_ptr<const EntityTemplate>& entityTemplate, EntityId id);
		~Entity() override;
		void operator=(const Entity&) = delete;

		bool isEntity() const override { return true; }

		inline EntityId getId() const { return m_id; }
		inline EntityHandle getHandle() const { return EntityHandle(this); }
		
		void setPosition(const flat::Vector3& position);
		void setPosition2d(const flat::Vector2& position2d);
		void setZ(float z);

		void setPositionSweep(const flat::Vector3& position);
		void setPosition2dSweep(const flat::Vector2& position2d);

		inline const flat::Vector3& getPosition() const { return m_position; }
		inline const flat::Vector2& getPosition2d() const { return reinterpret_cast<const flat::Vector2&>(m_position); }
		flat::Vector3 getCenter() const;
		
		inline const map::Map* getMap() const { return m_map; }
		inline map::Map* getMap() { return m_map; }
		
		void setHeading(float heading, float epsilon = 0.f);
		inline float getHeading() const { return m_heading; }

		void setElevation(float elevation);
		inline float getElevation() const { return m_elevation; }

		inline void setInstigator(Entity* instigator) { m_instigator = instigator; }
		inline Entity* getInstigator() const { return m_instigator.getEntity(); }

		inline flat::Vector3 getForward() const
		{
			flat::Vector3 forward;
			const float ce = std::cos(m_elevation);
			forward.x = std::cos(m_heading) * ce;
			forward.y = std::sin(m_heading) * ce;
			forward.z = std::sin(m_elevation);
			return forward;
		}
		
		inline void setSprite(flat::render::Sprite* sprite) { FLAT_ASSERT(sprite != nullptr); m_sprite = sprite; }
		inline void clearSprite() { m_sprite = nullptr; }
		inline bool hasSprite() const { return m_sprite != nullptr; }
		flat::render::BaseSprite& getSprite() override;
		void pushAttachedSprites(std::vector<const MapObject*>& objects) const;
		const flat::render::ProgramSettings& getProgramSettings() const override;
		
		bool addToMap(map::Map* map, EntityUpdater* entityUpdater);
		void removeFromMap();

#ifdef FLAT_DEBUG
		void debugDraw(debug::DebugDisplay& debugDisplay) const;
#endif
		
		bool isBusy() const;
		bool isBusy(component::ComponentFlags componentFlags) const;
		void cancelCurrentActions();
		void cancelCurrentActions(component::ComponentFlags componentFlags);
		
		// movement
		bool acceptsMoveOrders() const;
		bool acceptsPlayerMoveOrder(const flat::Vector2& point, Entity* interactionEntity = nullptr) const;
		void moveTo(const flat::Vector2& point, Entity* interactionEntity = nullptr, bool allowPartialPath = true);
		
		// behavior
		bool canInteract() const;
		void enterState(const char* stateName);
		void setInteractionIfCompatible(const char* stateName, entity::Entity* interactionEntity);

		void addComponent(component::Component* component);
		void cacheComponents();
		void initComponents();
		void deinitComponents();

		void reset();

		template <class ComponentType>
		inline const ComponentType* getComponent() const;

		template <class ComponentType>
		inline ComponentType* getComponent();

		template <class ComponentType>
		inline bool hasComponent() const { return getComponent<ComponentType>() != nullptr; }

		const component::Component* findComponent(component::ComponentFlags componentFlag) const;
		component::Component* findComponent(component::ComponentFlags componentFlag);

		inline const std::vector<component::Component*>& getComponents() const { return m_components; }
		inline void removeAllComponents() { m_components.clear(); }

		inline flat::lua::UniqueLuaReference<LUA_TTABLE>& getExtraData() { return m_extraData; }

		inline const std::shared_ptr<const EntityTemplate>& getEntityTemplate() const { return m_template; }
		const std::filesystem::path& getTemplatePath() const;
		const std::filesystem::path getTemplateName() const;

		template <class ComponentType>
		inline const typename ComponentType::TemplateType* getComponentTemplate() const;

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
		void updateAABB();
		void updateAABBIfDirty();
		inline void setAABBDirty() { m_aabbDirty = true; }

#ifdef FLAT_DEBUG
		void checkSpriteAABB();
		void checkValidPosition(const flat::Vector3& potentialPosition) const;
		void checkValidPosition2d(const flat::Vector2& potentialPosition) const;
#endif

		map::TileIndex getTileIndexFromPosition() const;

		inline void setGhost() { m_isGhost = true; }
		inline bool isGhost() const { return m_isGhost; }
		
	public:
		flat::Slot<const flat::Vector3&> positionChanged;
		flat::Slot<float> headingChanged;
		flat::Slot<float> elevationChanged;
		flat::Slot<Entity*, map::Map*, EntityUpdater*> addedToMap;
		flat::Slot<Entity*> removedFromMap;
		
	protected:
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
		component::sprite::SpriteComponent*       m_spriteComponent;

		flat::render::Sprite* m_sprite;
		
		EntityId m_id;

		flat::Vector3 m_position;
		float m_heading;
		float m_elevation;

		flat::lua::UniqueLuaReference<LUA_TTABLE> m_extraData;

		EntityHandle m_instigator;
		
		map::Map* m_map;
		int m_cellIndex;
		
		std::shared_ptr<const EntityTemplate> m_template;

		bool m_canBeSelected : 1;
		bool m_selected : 1;
		bool m_markedForDelete : 1;
		bool m_aabbDirty : 1;
		bool m_aabbCanChange : 1;
		bool m_isGhost : 1;
#ifdef FLAT_DEBUG
		bool m_debug : 1;
#endif
};

template <class ComponentType>
const typename ComponentType::TemplateType* game::entity::Entity::getComponentTemplate() const
{
	return m_template->getComponentTemplate<ComponentType>();
}

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

template <>
inline const component::sprite::SpriteComponent* Entity::getComponent() const
{
	return m_spriteComponent;
}
template <>
inline component::sprite::SpriteComponent* Entity::getComponent()
{
	return m_spriteComponent;
}

} // entity
} // game

#endif // GAME_ENTITY_ENTITY_H



