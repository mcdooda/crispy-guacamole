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
	m_owner->elevationChanged.on(this, &TextureComponent::elevationChanged);
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

	if (m_headingChanged || m_elevationChanged)
	{
		const float heading = m_owner->getHeading();
		FLAT_ASSERT(0.f <= heading && heading < flat::PI2);
		const float elevation = m_owner->getElevation();
		FLAT_ASSERT(-flat::PI <= elevation && elevation < flat::PI);

		flat::Vector3 direction(std::cos(heading), std::sin(heading), std::sin(elevation));

		const map::Map* map = m_owner->getMap();
		FLAT_ASSERT(map != nullptr);

		const flat::Vector2& xAxis = map->getXAxis();
		const flat::Vector2& yAxis = map->getYAxis();
		const flat::Vector2& zAxis = map->getZAxis();

		flat::Vector2 direction2d = xAxis * direction.x + yAxis * direction.y + zAxis * direction.z;
		m_sprite.setRotationZ(flat::vector2_angle(direction2d));

		m_headingChanged = false;
		m_elevationChanged = false;
	}
}

void TextureComponent::headingChanged(float heading)
{
	m_headingChanged = true;
}

void TextureComponent::elevationChanged(float elevation)
{
	m_elevationChanged = true;
}

void TextureComponent::positionChanged(const flat::Vector3 & position)
{
	m_positionChanged = true;
}

} // texture
} // component
} // entity
} // game



