#ifndef GAME_MAP_PROP_H
#define GAME_MAP_PROP_H

#include <flat.h>
#include "mapobject.h"
#include "tile.h"

namespace game
{
namespace map
{

class Prop final : public MapObject
{
public:
	Prop() : m_tileIndex(TileIndex::INVALID_VALUE) {}

	flat::render::BaseSprite* getSprite() override;
	using MapObject::getSprite;

	flat::render::Mesh* getMesh() override;
	using MapObject::getMesh;

	const flat::render::ProgramSettings& getProgramSettings() const override;

	void updateWorldSpaceAABB(const flat::Vector3& position);

	inline void setSpriteTexture(const std::shared_ptr<const flat::video::Texture>& propTexture) { m_sprite.setTexture(propTexture); m_sprite.getAABB(m_spriteAABB); }
	inline void setSpritePosition(const flat::Vector3& spritePosition) { m_sprite.setPosition(spritePosition); m_sprite.getAABB(m_spriteAABB); }
	inline void setSpriteOrigin(const flat::Vector2& spriteOrigin) { m_sprite.setOrigin(spriteOrigin); m_sprite.getAABB(m_spriteAABB); }
	inline void setSpriteColor(const flat::video::Color& spriteColor) { m_sprite.setColor(spriteColor); }

	inline void setMeshPosition(const flat::Vector3& meshPosition) { m_mesh.setPosition(meshPosition); }

	inline void setTileIndex(TileIndex tileIndex) { m_tileIndex = tileIndex; }
	inline TileIndex getTileIndex() const { return m_tileIndex; }

	void initializeMesh(const Map& map, const std::shared_ptr<const flat::video::Texture>& texture);

private:
	flat::render::Sprite m_sprite;
	flat::render::Mesh m_mesh; // TODO: share vertices between props using the same texture
	TileIndex m_tileIndex;
};

} // map
} // game

#endif // GAME_MAP_PROP_H



