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
class BehaviorComponent;
class MovementComponent;
class SpriteComponent;
}

class Entity final : public map::MapObject
{
	public:
		Entity(const std::shared_ptr<const EntityTemplate>& entityTemplate, lua_State* L);
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
		
		void draw(const flat::util::RenderSettings& renderSettings, const flat::Matrix4& viewMatrix) const override;
		
		void onAddedToMap(map::Map* map);
		void onRemovedFromMap();
		
		void update(float currentTime, float elapsedTime);
		
		bool isBusy() const;
		
		void addPointOnPath(const flat::Vector2& point);
		
		void enterState(const char* stateName);
		bool playAnimation(const char* animationName, int numLoops = 1);

		template <class ComponentType>
		inline const ComponentType* getComponent() const;
		template <class ComponentType>
		inline ComponentType* getComponent();
		
	public:
		flat::Slot<const flat::Vector3&> positionChanged;
		flat::Slot<float> headingChanged;
		flat::Slot<map::Map*> addedToMap;
		flat::Slot<> movementStarted;
		flat::Slot<> movementStopped;
		
	protected:
		void addComponent(component::Component* component);
		
		map::Tile* getTileFromPosition();
		
		void destroyComponents();
		
	protected:
		std::vector<component::Component*> m_components;
		
		component::BehaviorComponent* m_behaviorComponent;
		component::MovementComponent* m_movementComponent;
		component::SpriteComponent*   m_spriteComponent;
		
		flat::Vector3 m_position;
		float m_heading;
		
		map::Map* m_map;
		map::Tile* m_tile;
		
		std::shared_ptr<const EntityTemplate> m_template;
};

template <class ComponentType>
inline const ComponentType* Entity::getComponent() const
{
	static_assert(std::is_base_of<component::Component, std::decay<ComponentType>::type>::value, "");
	for (const component::Component* component : m_components)
	{
		if (const ComponentType* c = dynamic_cast<const ComponentType*>(component))
			return c;
	}
	return nullptr;
}

template <class ComponentType>
inline ComponentType* Entity::getComponent()
{
	static_assert(std::is_base_of<component::Component, std::decay<ComponentType>::type>::value, "");
	for (component::Component* component : m_components)
	{
		if (ComponentType* c = dynamic_cast<ComponentType*>(component))
			return c;
	}
	return nullptr;
}

template <>
inline const component::MovementComponent* Entity::getComponent() const
{
	return m_movementComponent;
}
template <>
inline component::MovementComponent* Entity::getComponent()
{
	return m_movementComponent;
}

} // entity
} // game

#endif // GAME_MAP_ENTITY_H



