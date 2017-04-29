#include "mapobject.h"

namespace game
{
namespace map
{

std::uint32_t MapObject::getTextureHash() const
{
	const flat::render::Sprite& sprite = getSprite();
	const flat::video::Texture* texture = sprite.getTexture().get();
	return texture->getHash();
}

} // map
} // game


