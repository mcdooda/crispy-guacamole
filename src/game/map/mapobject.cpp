#include "mapobject.h"
#include "flat.h"

namespace game
{
namespace map
{

flat::render::BaseSprite* MapObject::getSprite()
{
	return nullptr;
}

flat::render::Mesh* MapObject::getMesh()
{
	return nullptr;
}

const flat::video::Texture* MapObject::getTexture() const
{
	if (const flat::render::BaseSprite* sprite = getSprite())
	{
		return sprite->getTexture().get();
	}
	else if (const flat::render::Mesh* mesh = getMesh())
	{
		return mesh->getTexture().get();
	}
	return nullptr;
}

void MapObject::updateRenderHash() const
{
	const flat::video::Texture* texture = getTexture();
	FLAT_ASSERT_MSG(texture != nullptr && texture->getHash() != 0, "Map object's sprite does not have a texture");
	const flat::render::ProgramSettings& programSettings = getProgramSettings();
	const flat::video::Program& program = programSettings.program;
	FLAT_ASSERT_MSG(program.getHash() != 0, "Map object's program is not valid");
	const flat::render::RenderSettings& renderSettings = programSettings.settings;
	FLAT_ASSERT_MSG(renderSettings.getHash() != 0, "Map object's render settings are not valid");
	m_renderHash = texture->getHash() * 7 + program.getHash() * 11 + renderSettings.getHash() * 13;
}

} // map
} // game


