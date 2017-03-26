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

		const flat::Vector3& position = m_owner->getPosition();

		flat::Vector2 position2d(map->getTransform() * position);
		m_sprite.setPosition(position2d);

		m_owner->updateAABB();

		m_positionChanged = false;
	}

	if (m_headingChanged || m_elevationChanged)
	{
		const float heading = m_owner->getHeading();
		FLAT_ASSERT(0.f <= heading && heading < flat::PI2);
		const float elevation = m_owner->getElevation();
		FLAT_ASSERT(-flat::PI <= elevation && elevation < flat::PI);

		const float directionLenXY = std::cos(elevation);
		flat::Vector3 direction(std::cos(heading) * directionLenXY, std::sin(heading) * directionLenXY, std::sin(elevation));

		const map::Map* map = m_owner->getMap();
		FLAT_ASSERT(map != nullptr);

		flat::Vector2 direction2d(map->getTransform() * direction);
		m_sprite.setRotationZ(flat::vector2_angle(direction2d));

		m_headingChanged = false;
		m_elevationChanged = false;
	}
}

bool TextureComponent::headingChanged(float heading)
{
	m_headingChanged = true;
	return true;
}

bool TextureComponent::elevationChanged(float elevation)
{
	m_elevationChanged = true;
	return true;
}

bool TextureComponent::positionChanged(const flat::Vector3 & position)
{
	m_positionChanged = true;
	return true;
}

} // texture
} // component
} // entity
} // game



