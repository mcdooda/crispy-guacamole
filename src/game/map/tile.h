#ifndef GAME_MAP_TILE_H
#define GAME_MAP_TILE_H

#include <functional>
#include <memory>
#include <flat.h>
#include "mapobject.h"
#include "navigability.h"

namespace game
{
namespace map
{
class Map;
class Prop;

enum TileIndex : std::uint32_t
{
	INVALID = 0xFFFFFFFF
};

class Tile final : public MapObject
{
	public:
		Tile();
		Tile(const Tile&) = delete;
		Tile(Tile&& tile);
		~Tile() override;

		void synchronizeSpriteTo(const Map& map, flat::render::SpriteSynchronizer& spriteSynchronizer);

		bool isTile() const override { return true; }
		
		inline Navigability getNavigability() const { return m_navigability; }
		inline void setNavigability(Navigability navigability) { m_navigability = navigability; }
		inline bool isNavigable(Navigability navigabilityMask) const { return (m_navigability & navigabilityMask) != 0; }
		
		inline bool hasSprite() const { return m_sprite.getTexture() != nullptr; }

		flat::render::BaseSprite& getSprite() override;
		using MapObject::getSprite;

		const flat::render::ProgramSettings& getProgramSettings() const override;
		void updateWorldSpaceAABB();
		
		void setCoordinates(Map& map, const flat::Vector2i& xy, float z, bool init = false);
		inline const flat::Vector2i& getXY() const { return m_xy; }
		inline float getZ() const { return m_z; }
		inline void setZ(Map& map, float z) { setCoordinates(map, m_xy, z); }
		
		void setTexture(Map& map, const std::shared_ptr<const flat::video::Texture>& tileTexture);
		void setPropTexture(Map& map, const std::shared_ptr<const flat::video::Texture>& propTexture);
		void removeProp(Map& map);
		inline const Prop* getProp() const { return m_prop; }
		
		void setColor(const flat::video::Color& color);
		const flat::video::Color& getColor() const;

		inline static void setTileProgramSettings(const flat::render::ProgramSettings& programSettings)
		{
			tileProgramSettings = &programSettings;
		}

		inline static const flat::render::ProgramSettings& getTileProgramSettings()
		{
			FLAT_ASSERT(tileProgramSettings != nullptr);
			return *tileProgramSettings;
		}

	private:
		static const flat::render::ProgramSettings* tileProgramSettings;

		flat::render::SynchronizedSprite m_sprite;
		Prop* m_prop;
		
		flat::Vector2i m_xy;
		float m_z;

		Navigability m_navigability;
};

} // map
} // game

#endif // GAME_MAP_TILE_H



