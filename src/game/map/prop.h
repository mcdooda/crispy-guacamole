#ifndef GAME_MAP_PROP_H
#define GAME_MAP_PROP_H

#include <flat.h>
#include "mapobject.h"

namespace game
{
namespace map
{

class Prop : public MapObject
{
public:
	Prop() {}

	const flat::render::Sprite& getSprite() const override;

	inline void setSpriteTexture(const std::shared_ptr<const flat::video::Texture>& propTexture) { m_sprite.setTexture(propTexture); }
	inline void setSpritePosition(const flat::Vector2& spritePosition) { m_sprite.setPosition(spritePosition); }
	inline void setSpriteOrigin(const flat::Vector2& spriteOrigin) { m_sprite.setOrigin(spriteOrigin); }
	inline void setSpriteColor(const flat::video::Color& spriteColor) { m_sprite.setColor(spriteColor); }

	inline void setMapObjectZ(float z) { m_z = z; }
	float getMapObjectZ() const override { return m_z; }

private:
	float m_z;
	flat::render::Sprite m_sprite;
};

} // map
} // game

#endif // GAME_MAP_PROP_H



