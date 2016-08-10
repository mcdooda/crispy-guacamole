#ifndef GAME_MAP_ENTITY_H
#define GAME_MAP_ENTITY_H

#include <memory>
#include "../map/mapobject.h"
#include "component/behaviorcomponent.h"
#include "component/movementcomponent.h"
#include "component/spritecomponent.h"

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

class Entity final : public map::MapObject
{
	public:
		Entity(std::shared_ptr<const EntityTemplate> entityTemplate, lua_State* L);
		~Entity() override;
		
		inline std::shared_ptr<const EntityTemplate> getEntityTemplate() const { return m_template; }
		
		void setPosition(const flat::geometry::Vector3& position);
		inline const flat::geometry::Vector3& getPosition() const { return m_position; }
		
		inline const map::Map* getMap() const { return m_map; }
		inline const map::Tile* getTile() const { return m_tile; }
		
		void setHeading(float heading);
		inline float getHeading() const { return m_heading; }
		
		void draw(const flat::util::RenderSettings& renderSettings, const flat::geometry::Matrix4& viewMatrix) const override;
		
		void onAddedToMap(map::Map* map);
		void onRemovedFromMap();
		
		void update(float currentTime, float elapsedTime);
		
		bool followsPath() const { return m_movementComponent.followsPath(); }
		void addPointOnPath(const flat::geometry::Vector2& point) { m_movementComponent.addPointOnPath(point); }
		
		void enterState(const char* stateName);
		
		void setSpriteColumn(int column);
		
	protected:
		void registerComponent(component::Component& component);
		
		map::Tile* getTileFromPosition();
		void updateSpritePosition();
		
	protected:
		std::vector<component::Component*> m_components;
		
		component::BehaviorComponent m_behaviorComponent;
		component::MovementComponent m_movementComponent;
		component::SpriteComponent   m_spriteComponent;
		
		flat::geometry::Vector3 m_position;
		float m_heading;
		
		map::Map* m_map;
		map::Tile* m_tile;
		
		std::shared_ptr<const EntityTemplate> m_template;
};

} // entity
} // game

#endif // GAME_MAP_ENTITY_H



