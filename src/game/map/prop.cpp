#include "prop.h"
#include "tile.h"

namespace game
{
namespace map
{

flat::render::Sprite& Prop::getSprite()
{
	return m_sprite;
}

const flat::render::ProgramSettings& Prop::getProgramSettings() const
{
	return Tile::getTileProgramSettings();
}

void Prop::updateWorldSpaceAABB(const flat::Vector3& position)
{
	m_worldSpaceAABB.min = position - flat::Vector3(0.5f, 0.5f, 0.f);
	m_worldSpaceAABB.max = position + flat::Vector3(0.5f, 0.5f, 100.f);
}

}
}