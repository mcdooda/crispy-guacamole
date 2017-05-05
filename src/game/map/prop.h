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

	void setAABB(const flat::Vector3& position);

	inline void setSpriteTexture(const std::shared_ptr<const flat::video::Texture>& propTexture) { m_sprite.setTexture(propTexture); }
	inline void setSpritePosition(const flat::Vector2& spritePosition) { m_sprite.setPosition(spritePosition); m_sprite.getAABB(m_spriteAABB); }
	inline void setSpriteOrigin(const flat::Vector2& spriteOrigin) { m_sprite.setOrigin(spriteOrigin); }
	inline void setSpriteColor(const flat::video::Color& spriteColor) { m_sprite.setColor(spriteColor); }

private:
	flat::render::Sprite m_sprite;
};

} // map
} // game

#endif // GAME_MAP_PROP_H



