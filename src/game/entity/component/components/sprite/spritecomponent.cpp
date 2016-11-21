#include <iostream>
#include "spritecomponent.h"
#include "spritecomponenttemplate.h"
#include "../../componenttype.h"
#include "../../components/movement/movementcomponent.h"
#include "../../components/collision/collisioncomponent.h"
#include "../../../entity.h"
#include "../../../../map/map.h"

namespace game
{
namespace entity
{
namespace component
{
namespace sprite
{

void SpriteComponent::init()
{
	const sprite::SpriteDescription& spriteDescription = getTemplate()->getSpriteDescription();
	m_sprite.setTexture(spriteDescription.getAtlas());
	m_owner->setTextureHash(spriteDescription.getAtlas().get()->getHash());
	m_sprite.setOrigin(spriteDescription.getOrigin());
	m_sprite.setAtlasSize(spriteDescription.getAtlasWidth(), spriteDescription.getAtlasHeight());
	
	m_currentAnimationDescription = nullptr;

	if (setDefaultMoveAnimation())
	{
		m_sprite.setLine(m_moveAnimationDescription->getLine());
	}
	
	m_positionChanged = false;
	m_headingChanged = false;
	m_movementStarted = false;
	m_movementStopped = false;
	
	m_owner->headingChanged.on(this, &SpriteComponent::headingChanged);
	m_owner->positionChanged.on(this, &SpriteComponent::positionChanged);

	m_owner->selected.on(this, &SpriteComponent::selected);
	m_owner->deselected.on(this, &SpriteComponent::deselected);

	if (m_moveAnimationDescription)
	{
		if (movement::MovementComponent* movementComponent = m_owner->getComponent<movement::MovementComponent>())
		{
			movementComponent->movementStarted.on(this, &SpriteComponent::movementStarted);
			movementComponent->movementStopped.on(this, &SpriteComponent::movementStopped);
		}
	}
}

void SpriteComponent::playAnimation(const sprite::AnimationDescription& animationDescription, int numLoops)
{
	if (numLoops == flat::render::AnimatedSprite::INFINITE_LOOP
		&& m_sprite.getLastUpdateTime() > 0.f
		&& m_currentAnimationDescription == &animationDescription)
	{
		// avoid animation hiccups and continue the animation from the same frame
		m_sprite.setNumLoops(flat::render::AnimatedSprite::INFINITE_LOOP);
		m_sprite.setAnimated(true);
	}
	else
	{
		m_sprite.playAnimation(
			animationDescription.getLine(),
			animationDescription.getNumFrames(),
			animationDescription.getFrameDuration(),
			numLoops
		);
	}
	m_currentAnimationDescription = &animationDescription;
}

bool SpriteComponent::playAnimationByName(const std::string& animationName, int numLoops)
{
	const sprite::SpriteDescription& spriteDescription = getTemplate()->getSpriteDescription();
	const AnimationDescription* animationDescription = spriteDescription.getAnimationDescription(animationName);
	if (animationDescription)
	{
		playAnimation(*animationDescription, numLoops);
		return true;
	}
	return false;
}

bool SpriteComponent::setMoveAnimationByName(const std::string & moveAnimationName)
{
	const sprite::SpriteDescription& spriteDescription = getTemplate()->getSpriteDescription();
	const AnimationDescription* moveAnimationDescription = spriteDescription.getAnimationDescription(moveAnimationName);
	if (moveAnimationDescription)
	{
		m_moveAnimationDescription = moveAnimationDescription;
		return true;
	}
	return false;
}

bool SpriteComponent::setDefaultMoveAnimation()
{
	const sprite::SpriteDescription& spriteDescription = getTemplate()->getSpriteDescription();
	const AnimationDescription* defaultMoveAnimationDescription = spriteDescription.getDefaultMoveAnimationDescription();
	if (defaultMoveAnimationDescription)
	{
		m_moveAnimationDescription = defaultMoveAnimationDescription;
		return true;
	}
	return false;
}

void SpriteComponent::update(float currentTime, float elapsedTime)
{
	if (m_positionChanged)
	{
		const map::Map* map = m_owner->getMap();
		FLAT_ASSERT(map != nullptr);
		
		const flat::Vector3& position = m_owner->getPosition();
	
		flat::Vector2 position2d(map->getTransform() * position);
		m_sprite.setPosition(position2d);

		const collision::CollisionComponentTemplate* collisionComponentTemplate = getTemplate<collision::CollisionComponent>();
		const float radius = collisionComponentTemplate ? collisionComponentTemplate->getRadius() : 0.f;
		m_owner->computeDepth(position.x, position.y, radius);
		
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
		if (m_moveAnimationDescription)
		{
			playAnimation(*m_moveAnimationDescription, flat::render::AnimatedSprite::INFINITE_LOOP);
		}
		
		m_movementStarted = false;
	}
	
	if (m_movementStopped)
	{
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

void SpriteComponent::selected()
{
	m_sprite.setColor(flat::video::Color::RED);
}

void SpriteComponent::deselected()
{
	m_sprite.setColor(flat::video::Color::WHITE);
}

} // sprite
} // component
} // entity
} // game



