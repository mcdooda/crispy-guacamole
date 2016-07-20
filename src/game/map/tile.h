#ifndef GAME_MAP_TILE_H
#define GAME_MAP_TILE_H

#include <memory>
#include <flat.h>
#include "mapobject.h"

namespace game
{
namespace map
{
class Map;
class Entity;

class Tile final : public MapObject
{
	public:
		Tile();
		~Tile() override;
		
		inline bool exists() const { return m_exists; }
		inline void setExists(bool exists) { m_exists = exists; }
		
		void draw(const flat::util::RenderSettings& renderSettings, const flat::geometry::Matrix4& viewMatrix) const override;
		
		void setCoordinates(const Map& map, int x, int y, float z);
		
		void setTexture(std::shared_ptr<const flat::video::Texture> tileTexture);
		void setDoodadTexture(std::shared_ptr<const flat::video::Texture> doodadTexture);
		void removeDoodad();
		
		void addEntity(Entity* entity);
		void removeEntity(Entity* entity);
		const std::vector<Entity*>& getEntities() const { return m_entities; }
	
	private:
		std::vector<Entity*> m_entities;
		flat::util::Sprite m_sprite;
		flat::util::Sprite* m_doodadSprite;
		
		int m_x;
		int m_y;
		float m_z;
		
		bool m_exists;
};

} // map
} // game

#endif // GAME_MAP_TILE_H



