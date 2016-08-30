#include "prop.h"

namespace game
{
namespace map
{

void Prop::draw(const flat::util::RenderSettings& renderSettings, const flat::Matrix4& viewMatrix) const
{
	m_sprite.draw(renderSettings, viewMatrix);
}

}
}