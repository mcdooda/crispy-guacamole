#include <iostream>
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
	const sprite::Description& spriteDescription = entityTemplatePtr->getSpriteDescription();
	m_sprite.setTexture(spriteDescription.getAtlas());
	m_sprite.setOrigin(spriteDescription.getOrigin());
	m_sprite.setAtlasSize(spriteDescription.getAtlasWidth(), spriteDescription.getAtlasHeight());
	if (const sprite::AnimationDescription* moveAnimationDescription = spriteDescription.getMoveAnimationDescription())
	{
		playAnimation(*moveAnimationDescription, flat::util::AnimatedSprite::INFINITE_LOOP);
	}
	else
	{
		m_sprite.setAnimated(false);
	}
	
	owner->headingChanged.on(this, &SpriteComponent::headingChanged);
	owner->positionChanged.on(this, &SpriteComponent::positionChanged);
	owner->movementStarted.on(this, &SpriteComponent::movementStarted);
	owner->movementStopped.on(this, &SpriteComponent::movementStopped);
}

void SpriteComponent::playAnimation(const sprite::AnimationDescription& animationDescription, int numLoops)
{
	m_sprite.playAnimation(
		animationDescription.getLine(),
		animationDescription.getNumFrames(),
		animationDescription.getFrameDuration(),
		numLoops
	);
}

void SpriteComponent::update(float currentTime, float elapsedTime)
{
	m_sprite.update(currentTime);
}

bool SpriteComponent::isBusy() const
{
	return m_sprite.isAnimated() && !m_sprite.hasInfiniteLoop();
}

void SpriteComponent::headingChanged(float heading)
{
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

void SpriteComponent::positionChanged(const flat::Vector3& position)
{
	const map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map);
	
	const flat::Vector2& xAxis = map->getXAxis();
	const flat::Vector2& yAxis = map->getYAxis();
	const flat::Vector2& zAxis = map->getZAxis();
	
	flat::Vector2 position2d = xAxis * position.x + yAxis * position.y + zAxis * position.z;
	m_sprite.setPosition(position2d);

	const EntityTemplate* entityTemplatePtr = m_owner->getEntityTemplate().get();
	FLAT_ASSERT(entityTemplatePtr);	
	m_owner->computeDepth(position.x, position.y, entityTemplatePtr->getRadius());
}

void SpriteComponent::movementStarted()
{
	const EntityTemplate* entityTemplatePtr = m_owner->getEntityTemplate().get();
	FLAT_ASSERT(entityTemplatePtr);
	const sprite::Description& spriteDescription = entityTemplatePtr->getSpriteDescription();
	if (const sprite::AnimationDescription* moveAnimationDescription = spriteDescription.getMoveAnimationDescription())
	{
		playAnimation(*moveAnimationDescription, flat::util::AnimatedSprite::INFINITE_LOOP);
	}
}

void SpriteComponent::movementStopped()
{
	m_sprite.setColumn(0);
	m_sprite.setAnimated(false);
}

void SpriteComponent::draw(const flat::util::RenderSettings& renderSettings, const flat::Matrix4& viewMatrix) const
{
	m_sprite.draw(renderSettings, viewMatrix);
}

} // component
} // entity
} // game



