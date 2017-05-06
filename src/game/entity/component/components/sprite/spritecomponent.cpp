#include <iostream>
#include "spritecomponent.h"
#include "spritecomponenttemplate.h"
#include "../../componenttype.h"
#include "../../components/attack/attackcomponent.h"
#include "../../components/movement/movementcomponent.h"
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
	m_sprite.setOrigin(spriteDescription.getOrigin());
	m_sprite.setAtlasSize(spriteDescription.getAtlasWidth(), spriteDescription.getAtlasHeight());
	m_owner->setSprite(m_sprite);

	m_currentAnimationDescription = nullptr;

	if (setDefaultMoveAnimation())
	{
		m_sprite.setLine(m_moveAnimationDescription->getLine());
	}

	m_owner->addedToMap.on(this, &SpriteComponent::addedToMap);

	m_owner->headingChanged.on(this, &SpriteComponent::headingChanged);
	m_owner->positionChanged.on(this, &SpriteComponent::positionChanged);

	m_owner->selected.on(this, &SpriteComponent::selected);
	m_owner->deselected.on(this, &SpriteComponent::deselected);

	if (m_moveAnimationDescription)
	{
		movement::MovementComponent* movementComponent = m_owner->getComponent<movement::MovementComponent>();
		if (movementComponent != nullptr)
		{
			movementComponent->movementStarted.on(this, &SpriteComponent::movementStarted);
			movementComponent->movementStopped.on(this, &SpriteComponent::movementStopped);
		}
	}

	attack::AttackComponent* attackComponent = m_owner->getComponent<attack::AttackComponent>();
	if (attackComponent != nullptr)
	{
		attackComponent->attackStopped.on(this, &SpriteComponent::attackStopped);
	}
}

void SpriteComponent::deinit()
{
	m_owner->clearSprite();

	m_owner->addedToMap.off(this);
	m_owner->removedFromMap.off(this);

	m_owner->headingChanged.off(this);
	m_owner->positionChanged.off(this);

	m_owner->selected.off(this);
	m_owner->deselected.off(this);

	if (m_moveAnimationDescription)
	{
		movement::MovementComponent* movementComponent = m_owner->getComponent<movement::MovementComponent>();
		if (movementComponent != nullptr)
		{
			movementComponent->movementStarted.off(this);
			movementComponent->movementStopped.off(this);
		}
	}

	attack::AttackComponent* attackComponent = m_owner->getComponent<attack::AttackComponent>();
	if (attackComponent != nullptr)
	{
		attackComponent->attackStopped.off(this);
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

bool SpriteComponent::getAttachPoint(const std::string& attachPointName, flat::Vector3& attachPoint) const
{
	const sprite::SpriteDescription& spriteDescription = getTemplate()->getSpriteDescription();
	if (const flat::Vector2* attachPoint2d = spriteDescription.getAttachPoint(attachPointName))
	{
		flat::Vector2 relativePosition2d = *attachPoint2d - m_sprite.getOrigin();
		const map::Map* map = m_owner->getMap();
		const flat::Matrix3& mapInvTransform = map->getInvTransform();

		if (m_sprite.getFlipX())
		{
			relativePosition2d.x = -relativePosition2d.x;
		}

		// screen to map position
		flat::Vector3 relativePosition3d = mapInvTransform * flat::Vector3(relativePosition2d, 1.f);
		attachPoint = m_owner->getPosition() + relativePosition3d;
		return true;
	}
	return false;
}

void SpriteComponent::update(float currentTime, float elapsedTime)
{
	bool updateAABB = false;

	if (m_positionChanged)
	{
		const map::Map* map = m_owner->getMap();
		FLAT_ASSERT(map != nullptr);
		
		const flat::Vector3& position = m_owner->getPosition();
	
		flat::Vector2 position2d(map->getTransform() * position);
		m_sprite.setPosition(position2d);
		updateAABB = true;
		
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
		updateAABB = true;
		
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
	else if (m_movementStopped)
	{
		m_sprite.setAnimated(false);
		
		m_movementStopped = false;
	}

	if (m_attackStopped)
	{
		if (m_moveAnimationDescription && m_owner->getComponent<movement::MovementComponent>()->followsPath())
		{
			playAnimation(*m_moveAnimationDescription, flat::render::AnimatedSprite::INFINITE_LOOP);
		}
		else
		{
			m_sprite.setAnimated(false);
		}

		m_attackStopped = false;
	}
	
	m_sprite.update(currentTime);

	if (updateAABB)
	{
		m_owner->updateAABB();
	}
}

bool SpriteComponent::isBusy() const
{
	return m_sprite.isAnimated() && !m_sprite.hasInfiniteLoop();
}

#ifdef FLAT_DEBUG
void SpriteComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	debugDisplay.add2dAABB(m_owner->getAABB());
}
#endif

bool SpriteComponent::addedToMap(Entity * entity, map::Map * map)
{
	m_positionChanged = false;
	m_headingChanged = false;
	m_movementStarted = false;
	m_movementStopped = false;
	m_attackStopped = false;

	return true;
}

bool SpriteComponent::headingChanged(float heading)
{
	m_headingChanged = true;
	return true;
}

bool SpriteComponent::positionChanged(const flat::Vector3& position)
{
	m_positionChanged = true;
	return true;
}

bool SpriteComponent::movementStarted()
{
	m_movementStarted = true;
	return true;
}

bool SpriteComponent::movementStopped()
{
	m_movementStopped = true;
	return true;
}

bool SpriteComponent::attackStopped()
{
	m_attackStopped = true;
	return true;
}

bool SpriteComponent::selected()
{
	m_sprite.setColor(flat::video::Color::RED);
	return true;
}

bool SpriteComponent::deselected()
{
	m_sprite.setColor(flat::video::Color::WHITE);
	return true;
}

} // sprite
} // component
} // entity
} // game



