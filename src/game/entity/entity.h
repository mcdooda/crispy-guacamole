#ifndef GAME_MAP_ENTITY_H
#define GAME_MAP_ENTITY_H

#include <memory>
#include <queue>
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
namespace behavior
{
class BehaviorRuntime;
}

class Entity final : public map::MapObject
{
	public:
		Entity(std::shared_ptr<const EntityTemplate> entityTemplate, lua_State* L);
		~Entity() override;
		
		inline std::shared_ptr<const EntityTemplate> getEntityTemplate() const { return m_template; }
		
		void setPosition(const flat::geometry::Vector3& position);
		inline const flat::geometry::Vector3& getPosition() const { return m_position; }
		
		void setHeading(float heading);
		inline float getHeading() const { return m_heading; }
		
		void draw(const flat::util::RenderSettings& renderSettings, const flat::geometry::Matrix4& viewMatrix) const override;
		
		void onAddedToMap(map::Map* map);
		void onRemovedFromMap();
		
		void update(float currentTime, float elapsedTime);
		
		bool followsPath() const { return !m_path.empty(); }
		void addPointOnPath(const flat::geometry::Vector2& point);
		
		void enterState(const char* stateName);
		
	protected:
		map::Tile* getTileFromPosition();
		void updateSpritePosition();
		
		void updateSprite(float currentTime);
		void updateBehavior();
		void followPath(float elapsedTime);
		
	protected:
		flat::util::AnimatedSprite m_sprite;
		
		flat::geometry::Vector3 m_position;
		float m_heading;
		std::queue<flat::geometry::Vector2> m_path;
		
		map::Map* m_map;
		map::Tile* m_tile;
		
		std::shared_ptr<const EntityTemplate> m_template;
		
		behavior::BehaviorRuntime* m_behaviorRuntime;
};

} // entity
} // game

#endif // GAME_MAP_ENTITY_H



