#ifndef GAME_MAP_TILE_H
#define GAME_MAP_TILE_H

#include <memory>
#include <flat.h>
#include "mapobject.h"

namespace game
{
namespace entity
{
class Entity;
}
namespace map
{
class Map;

class Tile final : public MapObject
{
	public:
		Tile();
		~Tile() override;
		
		inline bool exists() const { return m_exists; }
		inline void setExists(bool exists) { m_exists = exists; }
		
		inline bool isWalkable() const { return m_walkable; }
		inline void setWalkable(bool walkable) { m_walkable = walkable; }
		
		void draw(const flat::util::RenderSettings& renderSettings, const flat::Matrix4& viewMatrix) const override;
		
		void setCoordinates(const Map& map, int x, int y, float z);
		inline int getX() const { return m_x; }
		inline int getY() const { return m_y; }
		inline float getZ() const { return m_z; }
		
		void setTexture(std::shared_ptr<const flat::video::Texture> tileTexture);
		void setPropTexture(std::shared_ptr<const flat::video::Texture> propTexture);
		void removeProp();
		
		void addEntity(entity::Entity* entity);
		void removeEntity(entity::Entity* entity);
		const std::vector<entity::Entity*>& getEntities() const { return m_entities; }
		
		void setColor(const flat::video::Color& color);
		
		void getWalkableNeighborTiles(const Map& map, float jumpHeight, std::vector<const Tile*>& neighborTiles) const;
	
	private:
		std::vector<entity::Entity*> m_entities;
		flat::util::Sprite m_sprite;
		flat::util::Sprite* m_propSprite;
		
		int m_x;
		int m_y;
		float m_z;
		
		bool m_exists : 1;
		bool m_walkable : 1;
};

} // map
} // game

#endif // GAME_MAP_TILE_H



