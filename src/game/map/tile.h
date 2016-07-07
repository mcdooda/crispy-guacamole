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

class Tile : public MapObject
{
	public:
		Tile();
		virtual ~Tile();
		
		inline bool exists() const { return m_exists; }
		inline void setExists(bool exists) { m_exists = exists; }
		
		void draw(const flat::util::RenderSettings& renderSettings, const flat::geometry::Matrix4& viewMatrix) const override;
		
		void setCoordinates(const Map& map, int x, int y, float z);
		
		inline void setZ(float z) { m_z = z; }
		inline float getZ() const { return m_z; }
		
		void setTexture(std::shared_ptr<const flat::video::Texture> tileTexture);
		void setDoodadTexture(std::shared_ptr<const flat::video::Texture> doodadTexture);
		void removeDoodad();
	
	private:
		int m_x;
		int m_y;
		float m_z;
		
		bool m_exists;
		
		flat::util::Sprite m_sprite;
		flat::util::Sprite* m_doodadSprite;
};

} // map
} // game

#endif // GAME_MAP_TILE_H



