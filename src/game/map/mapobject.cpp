#include "mapobject.h"

namespace game
{
namespace map
{

std::uint32_t MapObject::getTextureHash() const
{
	const flat::render::Sprite& sprite = getSprite();
	const flat::video::Texture* texture = sprite.getTexture().get();
	FLAT_ASSERT_MSG(texture != nullptr, "Map object's sprite does not have a texture");
	return texture->getHash();
}

} // map
} // game


