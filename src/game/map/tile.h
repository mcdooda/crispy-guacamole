#ifndef GAME_MAP_TILE_H
#define GAME_MAP_TILE_H

#include <functional>
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
class Prop;

class Tile final : public MapObject
{
	public:
		Tile();
		~Tile() override;

		bool isTile() const override { return true; }
		
		inline bool exists() const { return m_exists; }
		void setExists(Map& map, bool exists);
		
		inline bool isWalkable() const { return m_walkable; }
		inline void setWalkable(bool walkable) { m_walkable = walkable; }
		
		inline bool hasSprite() const { return static_cast<bool>(m_sprite.getTexture()); }
		const flat::render::Sprite& getSprite() const override;
		const flat::render::ProgramSettings& getProgramSettings() const override;
		void updateWorldSpaceAABB();
		
		void setCoordinates(Map& map, int x, int y, float z);
		inline int getX() const { return m_x; }
		inline int getY() const { return m_y; }
		inline float getZ() const { return m_z; }
		inline void setZ(Map& map, float z) { setCoordinates(map, m_x, m_y, z); }
		
		void setTexture(Map& map, const std::shared_ptr<const flat::video::Texture>& tileTexture);
		void setPropTexture(Map& map, const std::shared_ptr<const flat::video::Texture>& propTexture);
		void removeProp(Map& map);
		inline const Prop* getProp() const { return m_prop; }
		
		void addEntity(entity::Entity* entity);
		void removeEntity(entity::Entity* entity);
		const std::vector<entity::Entity*>& getEntities() const { return m_entities; }
		
		void setColor(const flat::video::Color& color);
		const flat::video::Color& getColor() const;
		
		void eachWalkableNeighborTiles(const Map& map, float jumpHeight, std::function<void(const Tile*)> func) const;

		inline static void setTileProgramSettings(const flat::render::ProgramSettings& programSettings)
		{
			tileProgramSettings = &programSettings;
		}

		inline static const flat::render::ProgramSettings& getTileProgramSettings()
		{
			FLAT_ASSERT(tileProgramSettings != nullptr);
			return *tileProgramSettings;
		}
	
		void updateNormal(Map& map);
		inline void setNormalDirty() { FLAT_ASSERT(!m_normalDirty); m_normalDirty = true; }
		inline bool isNormalDirty() const { return m_normalDirty; }

	private:
		void setNearbyTilesDirty(Map& map);

		static const flat::render::ProgramSettings* tileProgramSettings;

		std::vector<entity::Entity*> m_entities;
		flat::render::Sprite m_sprite;
		Prop* m_prop;
		
		int m_x;
		int m_y;
		float m_z;
		
		bool m_exists : 1;
		bool m_walkable : 1;

		bool m_normalDirty : 1;
};

} // map
} // game

#endif // GAME_MAP_TILE_H



