#include "spritecomponent.h"
#include "../entity.h"
#include "../entitytemplate.h"
#include "../../map/map.h"

namespace game
{
namespace entity
{
namespace component
{

void SpriteComponent::setOwner(Entity* owner)
{
	Super::setOwner(owner);
	const EntityTemplate* entityTemplatePtr = owner->getEntityTemplate().get();
	FLAT_ASSERT(entityTemplatePtr);
	m_sprite.setTexture(entityTemplatePtr->getAtlas());
	m_sprite.setOrigin(entityTemplatePtr->getSpriteOrigin());
	m_sprite.setAtlasSize(entityTemplatePtr->getAtlasWidth(), entityTemplatePtr->getAtlasHeight());
	m_sprite.setFrameDuration(entityTemplatePtr->getAnimationFrameDuration());
}

void SpriteComponent::update(float currentTime, float elapsedTime)
{
	m_sprite.setAnimated(m_owner->followsPath());
	m_sprite.update(currentTime);
}

void SpriteComponent::updateHeading()
{
	float heading = m_owner->getHeading();
	FLAT_ASSERT(0 <= heading && heading < M_PI * 2.f);
	if (heading >= M_PI / 4.f && heading <= 5.f * M_PI / 4.f)
	{
		m_sprite.setFlipX(false);
	}
	else
	{
		m_sprite.setFlipX(true);
	}
}

void SpriteComponent::updatePosition()
{
	const flat::geometry::Vector3& position = m_owner->getPosition();
	
	const map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map);
	
	const flat::geometry::Vector2 xAxis = map->getXAxis();
	const flat::geometry::Vector2 yAxis = map->getYAxis();
	const flat::geometry::Vector2 zAxis = map->getZAxis();
	
	flat::geometry::Vector2 position2d = xAxis * position.x + yAxis * position.y + zAxis * position.z;
	m_sprite.setPosition(position2d);

	const EntityTemplate* entityTemplatePtr = m_owner->getEntityTemplate().get();
	FLAT_ASSERT(entityTemplatePtr);	
	m_owner->computeDepth(position.x, position.y, entityTemplatePtr->getRadius());
}

void SpriteComponent::draw(const flat::util::RenderSettings& renderSettings, const flat::geometry::Matrix4& viewMatrix) const
{
	m_sprite.draw(renderSettings, viewMatrix);
}

} // component
} // entity
} // game



