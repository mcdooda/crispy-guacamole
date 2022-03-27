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
class TileTemplate;

using TileIndex = flat::sharp::ai::navigation::AreaId;

inline bool isValidTile(TileIndex tileIndex)
{
	return flat::sharp::ai::navigation::isValidArea(tileIndex);
}

enum PropIndex
{
	INVALID_PROP = 0xFFFFFFFF
};

inline bool isValidProp(PropIndex propIndex)
{
	return propIndex != PropIndex::INVALID_PROP;
}

class Tile final : public MapObject
{
	public:
		Tile();
		Tile(const Tile&) = default;
		Tile(Tile&& tile);
		~Tile() override;

		Tile& operator=(const Tile& tile) = default;
		Tile& operator=(Tile&& tile);

		void synchronizeSpriteTo(const Map& map, flat::render::SpriteSynchronizer& spriteSynchronizer);

		void initializeMesh(const Map& map, const TileTemplate& tileTemplate, uint16_t tileTemplateVariantIndex);

		bool isTile() const override { return true; }
		
		inline bool hasSprite() const { return m_sprite.getTexture() != nullptr; }

		flat::render::BaseSprite* getSprite() override;
		using MapObject::getSprite;

		flat::render::Mesh* getMesh() override;
		using MapObject::getMesh;

		const flat::render::ProgramSettings& getProgramSettings() const override;
		void updateWorldSpaceAABB(const flat::Vector3& position);

		inline void setSpritePosition(const flat::Vector3& spritePosition) { m_sprite.setPosition(spritePosition); m_sprite.getAABB(m_spriteAABB); }

		inline void setMeshPosition(const flat::Vector3& meshPosition) { m_mesh.setPosition(meshPosition); }

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
		flat::render::Mesh m_mesh; // TODO: share meshes between tiles of the same template
		PropIndex m_propIndex;
};

} // map
} // game

#endif // GAME_MAP_TILE_H