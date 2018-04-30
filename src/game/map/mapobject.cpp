#include "mapobject.h"

namespace game
{
namespace map
{

void MapObject::updateRenderHash() const
{
	const flat::render::BaseSprite& sprite = getSprite();
	const flat::video::Texture* texture = sprite.getTexture().get();
	FLAT_ASSERT_MSG(texture != nullptr, "Map object's sprite does not have a texture");
	const flat::render::ProgramSettings& programSettings = getProgramSettings();
	const flat::video::Program& program = programSettings.program;
	FLAT_ASSERT_MSG(program.getHash() != 0, "Map object's program is not valid");
	const flat::render::RenderSettings& renderSettings = programSettings.settings;
	FLAT_ASSERT_MSG(renderSettings.getHash() != 0, "Map object's render settings are not valid");
	m_renderHash = texture->getHash() * 7 + program.getHash() * 11 + renderSettings.getHash() * 13;
}

} // map
} // game


