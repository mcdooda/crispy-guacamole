#ifndef GAME_MAP_ENTITY_H
#define GAME_MAP_ENTITY_H

#include <memory>
#include "../map/mapobject.h"

namespace game
{
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
namespace behavior { class BehaviorComponent; }
namespace movement { class MovementComponent; }
namespace sprite   { class SpriteComponent; }
namespace texture  { class TextureComponent; }
}

class Entity final : public map::MapObject
{
	public:
		Entity(const std::shared_ptr<const EntityTemplate>& entityTemplate);
		~Entity() override;
		
		inline const std::shared_ptr<const EntityTemplate>& getEntityTemplate() const { return m_template; }
		
		void setPosition(const flat::Vector3& position);
		void setXY(const flat::Vector2& xy);
		void setZ(float z);
		inline const flat::Vector3& getPosition() const { return m_position; }
		
		inline const map::Map* getMap() const { return m_map; }
		inline const map::Tile* getTile() const { return m_tile; }
		
		void setHeading(float heading);
		inline float getHeading() const { return m_heading; }

		void setElevation(float elevation);
		inline float getElevation() const { return m_elevation; }
		
		const flat::render::Sprite& getSprite() const override;
		
		void onAddedToMap(map::Map* map);
		void onRemovedFromMap();
		
		void update(float currentTime, float elapsedTime);
		
		bool isBusy() const;
		
		void addPointOnPath(const flat::Vector2& point);
		
		void enterState(const char* stateName);
		bool playAnimation(const char* animationName, int numLoops = 1);

		void addComponent(component::Component* component);
		void cacheComponents();
		void initComponents();

		template <class ComponentType>
		inline const ComponentType* findComponent() const;
		template <class ComponentType>
		inline ComponentType* findComponent();

		template <class ComponentType>
		inline const ComponentType* getComponent() const;
		template <class ComponentType>
		inline ComponentType* getComponent();

		inline const std::vector<component::Component*>& getComponents() const { return m_components; }
		inline void removeAllComponents() { m_components.clear(); }

		inline flat::lua::SharedLuaReference<LUA_TTABLE>& getExtraData() { return m_extraData; }

		const std::string& getTemplateName() const;
		
	public:
		flat::Slot<const flat::Vector3&> positionChanged;
		flat::Slot<float> headingChanged;
		flat::Slot<float> elevationChanged;
		flat::Slot<map::Map*> addedToMap;
		
	protected:
		
		map::Tile* getTileFromPosition();
		
	protected:
		std::vector<component::Component*> m_components;
		component::behavior::BehaviorComponent* m_behaviorComponent;
		component::movement::MovementComponent* m_movementComponent;
		component::sprite::SpriteComponent*     m_spriteComponent;
		component::texture::TextureComponent*   m_textureComponent;
		
		flat::Vector3 m_position;
		float m_heading;
		float m_elevation;

		flat::lua::SharedLuaReference<LUA_TTABLE> m_extraData;
		
		map::Map* m_map;
		map::Tile* m_tile;
		
		std::shared_ptr<const EntityTemplate> m_template;
};

template <class ComponentType>
inline const ComponentType* Entity::findComponent() const
{
	static_assert(std::is_base_of<component::Component, ComponentType>::value, "ComponentType must inherit from Component");
	for (const component::Component* component : m_components)
	{
		if (const ComponentType* c = dynamic_cast<const ComponentType*>(component))
			return c;
	}
	return nullptr;
}

template <class ComponentType>
inline ComponentType* Entity::findComponent()
{
	static_assert(std::is_base_of<component::Component, ComponentType>::value, "ComponentType must inherit from Component");
	for (component::Component* component : m_components)
	{
		if (ComponentType* c = dynamic_cast<ComponentType*>(component))
			return c;
	}
	return nullptr;
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

#endif // GAME_MAP_ENTITY_H



