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
		Entity(std::shared_ptr<const EntityTemplate> entityTemplate, lua_State* L);
		~Entity() override;
		
		inline std::shared_ptr<const EntityTemplate> getEntityTemplate() const { return m_template; }
		
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
		
		inline component::MovementComponent* getMovementComponent() { return m_movementComponent; }
		
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

} // entity
} // game

#endif // GAME_MAP_ENTITY_H



