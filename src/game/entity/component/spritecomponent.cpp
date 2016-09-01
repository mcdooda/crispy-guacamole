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
	FLAT_ASSERT(entityTemplatePtr != nullptr);
	const sprite::Description& spriteDescription = entityTemplatePtr->getSpriteDescription();
	m_sprite.setTexture(spriteDescription.getAtlas());
	m_owner->setTextureHash(spriteDescription.getAtlas().get()->getHash());
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
	
	m_positionChanged = false;
	m_headingChanged = false;
	m_movementStarted = false;
	m_movementStopped = false;
	
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
	const EntityTemplate* entityTemplatePtr = m_owner->getEntityTemplate().get();
	FLAT_ASSERT(entityTemplatePtr != nullptr);
	
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

		m_owner->computeDepth(position.x, position.y, entityTemplatePtr->getRadius());
		
		m_positionChanged = false;
	}
	
	if (m_headingChanged)
	{
		const float heading = m_owner->getHeading();
		FLAT_ASSERT(0 <= heading && heading < M_PI * 2.f);
		if (heading >= M_PI / 4.f && heading <= 5.f * M_PI / 4.f)
		{
			m_sprite.setFlipX(false);
		}
		else
		{
			m_sprite.setFlipX(true);
		}
		
		m_headingChanged = false;
	}
	
	if (m_movementStarted)
	{
		const sprite::Description& spriteDescription = entityTemplatePtr->getSpriteDescription();
		if (const sprite::AnimationDescription* moveAnimationDescription = spriteDescription.getMoveAnimationDescription())
		{
			playAnimation(*moveAnimationDescription, flat::util::AnimatedSprite::INFINITE_LOOP);
		}
		
		m_movementStarted = false;
	}
	
	if (m_movementStopped)
	{
		m_sprite.setColumn(0);
		m_sprite.setAnimated(false);
		
		m_movementStopped = false;
	}
	
	m_sprite.update(currentTime);
}

bool SpriteComponent::isBusy() const
{
	return m_sprite.isAnimated() && !m_sprite.hasInfiniteLoop();
}

void SpriteComponent::headingChanged(float heading)
{
	m_headingChanged = true;
}

void SpriteComponent::positionChanged(const flat::Vector3& position)
{
	m_positionChanged = true;
}

void SpriteComponent::movementStarted()
{
	m_movementStarted = true;
}

void SpriteComponent::movementStopped()
{
	m_movementStopped = true;
}

} // component
} // entity
} // game



