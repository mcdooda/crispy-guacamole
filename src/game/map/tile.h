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

enum TileIndex
{
	INVALID_TILE = 0xFFFFFFFF
};

enum PropIndex
{
	INVALID_PROP = 0xFFFFFFFF
};

class Tile final : public MapObject
{
	public:
		Tile();
		Tile(const Tile&) = delete;
		Tile(Tile&& tile);
		~Tile() override;

		Tile& operator=(Tile&& tile);

		void synchronizeSpriteTo(const Map& map, flat::render::SpriteSynchronizer& spriteSynchronizer);

		bool isTile() const override { return true; }
		
		inline bool hasSprite() const { return m_sprite.getTexture() != nullptr; }

		flat::render::BaseSprite& getSprite() override;
		using MapObject::getSprite;

		const flat::render::ProgramSettings& getProgramSettings() const override;
		void updateWorldSpaceAABB(const flat::Vector3& position);

		inline void setSpritePosition(const flat::Vector2& spritePosition) { m_sprite.setPosition(spritePosition); m_sprite.getAABB(m_spriteAABB); }

		inline void setPropIndex(PropIndex propIndex) { m_propIndex = propIndex; }
		inline PropIndex getPropIndex() const { return m_propIndex; }

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
		PropIndex m_propIndex;
};

} // map
} // game

#endif // GAME_MAP_TILE_H