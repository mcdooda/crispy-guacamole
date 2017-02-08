#include "prop.h"

namespace game
{
namespace map
{

const flat::render::Sprite& Prop::getSprite() const
{
	return m_sprite;
}

void Prop::setAABB(const flat::Vector3& position)
{
	m_AABB.min = position - flat::Vector3(0.5f, 0.5f, 0.f);
	m_AABB.max = position + flat::Vector3(0.5f, 0.5f, 100.f);
}

}
}