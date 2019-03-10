#ifndef GAME_MAP_TILE_H
#define GAME_MAP_TILE_H

#include <functional>
#include <memory>
#include <flat.h>
#include "mapobject.h"
#include "navigability.h"

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

		void synchronizeSpriteTo(const Map& map, flat::render::SpriteSynchronizer& spriteSynchronizer);

		bool isTile() const override { return true; }
		
		inline bool exists() const { return m_exists; }
		void setExists(Map& map, bool exists);
		
		inline Navigability getNavigability() const { return m_navigability; }
		inline void setNavigability(Navigability navigability) { m_navigability = navigability; }
		inline bool isNavigable(Navigability navigabilityMask) const { return (m_navigability & navigabilityMask) != 0; }
		
		inline bool hasSprite() const { return m_sprite.getTexture() != nullptr; }

		flat::render::BaseSprite& getSprite() override;
		using MapObject::getSprite;

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
		
		void setColor(const flat::video::Color& color);
		const flat::video::Color& getColor() const;
		
		void eachNeighborTilesWithNavigability(const Map& map, float jumpHeight, Navigability navigabilityMask, std::function<void(const Tile*)> func) const;

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

		flat::render::SynchronizedSprite m_sprite;
		Prop* m_prop;
		
		int m_x;
		int m_y;
		float m_z;

		Navigability m_navigability;
		
		bool m_exists : 1;

		bool m_normalDirty : 1;
};

} // map
} // game

#endif // GAME_MAP_TILE_H



