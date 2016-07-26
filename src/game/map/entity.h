#ifndef GAME_MAP_ENTITY_H
#define GAME_MAP_ENTITY_H

#include <memory>
#include "mapobject.h"

namespace game
{
namespace map
{
class Map;
class Tile;
class EntityTemplate;

class Entity final : public MapObject
{
	public:
		Entity(std::shared_ptr<const EntityTemplate> entityTemplate);
		~Entity() override;
		
		void setPosition(const flat::geometry::Vector3& position);
		inline const flat::geometry::Vector3& getPosition() const { return m_position; }
		
		void setHeading(float heading);
		inline float getHeading() const { return m_heading; }
		
		void draw(const flat::util::RenderSettings& renderSettings, const flat::geometry::Matrix4& viewMatrix) const override;
		
		void onAddedToMap(Map* map);
		void onRemovedFromMap();
		
		void update(float currentTime);
		
	protected:
		Tile* getTileFromPosition();
		void updateSpritePosition();
		
	protected:
		flat::util::AnimatedSprite m_sprite;
		flat::geometry::Vector3 m_position;
		float m_heading;
		std::shared_ptr<const EntityTemplate> m_template;
		Map* m_map;
		Tile* m_tile;
};

} // map
} // game

#endif // GAME_MAP_ENTITY_H



