#include <iostream>
#include "texturecomponent.h"
#include "texturecomponenttemplate.h"
#include "../../../entity.h"
#include "../../../../map/map.h"

namespace game
{
namespace entity
{
namespace component
{
namespace texture
{

void TextureComponent::init()
{
	const TextureComponentTemplate* textureComponentTemplate = getTemplate();
	m_sprite.setTexture(textureComponentTemplate->getTexture());
	m_sprite.setOrigin(textureComponentTemplate->getOrigin());

	m_owner->headingChanged.on(this, &TextureComponent::headingChanged);
	m_owner->positionChanged.on(this, &TextureComponent::positionChanged);
}

void TextureComponent::update(float currentTime, float elapsedTime)
{
	if (m_positionChanged)
	{
		const map::Map* map = m_owner->getMap();
		FLAT_ASSERT(map != nullptr);

		const flat::Vector2& xAxis = map->getXAxis();
		const flat::Vector2& yAxis = map->getYAxis();
		const flat::Vector2& zAxis = map->getZAxis();

		const flat::Vector3& position = m_owner->getPosition();

		flat::Vector2 position2d = xAxis * position.x + yAxis * position.y + zAxis * position.z;
		m_sprite.setPosition(position2d);

		m_owner->computeDepth(position.x, position.y, 0.f);

		m_positionChanged = false;
	}

	if (m_headingChanged)
	{
		const float heading = m_owner->getHeading();
		FLAT_ASSERT(0 <= heading && heading < M_PI * 2.f);

		const float h = std::atan2f(std::sin(heading + flat::PI / 4.f) / 2.f, std::cos(heading + flat::PI / 4.f));	
		m_sprite.setRotationZ(h);

		m_headingChanged = false;
	}
}

void TextureComponent::headingChanged(float heading)
{
	m_headingChanged = true;
}

void TextureComponent::positionChanged(const flat::Vector3 & position)
{
	m_positionChanged = true;
}

} // texture
} // component
} // entity
} // game



