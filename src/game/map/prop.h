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
	Prop() : m_tileIndex(TileIndex::INVALID_TILE) {}

	flat::render::BaseSprite& getSprite() override;
	using MapObject::getSprite;

	const flat::render::ProgramSettings& getProgramSettings() const override;

	void updateWorldSpaceAABB(const flat::Vector3& position);

	inline void setSpriteTexture(const std::shared_ptr<const flat::video::Texture>& propTexture) { m_sprite.setTexture(propTexture); m_sprite.getAABB(m_spriteAABB); }
	inline void setSpritePosition(const flat::Vector2& spritePosition) { m_sprite.setPosition(spritePosition); m_sprite.getAABB(m_spriteAABB); }
	inline void setSpriteOrigin(const flat::Vector2& spriteOrigin) { m_sprite.setOrigin(spriteOrigin); m_sprite.getAABB(m_spriteAABB); }
	inline void setSpriteColor(const flat::video::Color& spriteColor) { m_sprite.setColor(spriteColor); }

	inline void setTileIndex(TileIndex tileIndex) { m_tileIndex = tileIndex; }
	inline TileIndex getTileIndex() const { return m_tileIndex; }

private:
	flat::render::Sprite m_sprite;
	TileIndex m_tileIndex;
};

} // map
} // game

#endif // GAME_MAP_PROP_H



