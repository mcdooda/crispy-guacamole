#include <iostream>
#include "spritecomponent.h"
#include "spritecomponenttemplate.h"
#include "../../componenttype.h"
#include "../../components/selection/selectioncomponent.h"
#include "../../../entity.h"
#include "../../../../map/map.h"
#include "../../../../map/displaymanager.h"
#include "../../../lua/entity.h"

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
	const SpriteDescription& spriteDescription = getTemplate()->getSpriteDescription();
	m_sprite.setTexture(spriteDescription.getAtlas());
	m_sprite.setOrigin(spriteDescription.getOrigin());
	m_sprite.setAtlasSize(spriteDescription.getAtlasWidth(), spriteDescription.getAtlasHeight());
	m_owner->setSprite(&m_sprite);

	m_cycleAnimationDescription = nullptr;
	m_currentAnimationDescription = nullptr;
	m_isCycleAnimated = true;

	selection::SelectionComponent* selectionComponent = m_owner->getComponent<selection::SelectionComponent>();
	if (selectionComponent != nullptr)
	{
		selectionComponent->selected.on(this, &SpriteComponent::selected);
		selectionComponent->deselected.on(this, &SpriteComponent::deselected);
	}

	m_owner->addedToMap.on(this, &SpriteComponent::addedToMap);

	const flat::lua::SharedLuaReference<LUA_TFUNCTION>& onInit = getTemplate()->getOnInit();
	if (!onInit.isEmpty())
	{
		onInit.call(m_owner);
	}

	m_preventBusy = false;
}

void SpriteComponent::deinit()
{
	m_owner->clearSprite();

	selection::SelectionComponent* selectionComponent = m_owner->getComponent<selection::SelectionComponent>();
	if (selectionComponent != nullptr)
	{
		selectionComponent->selected.off(this);
		selectionComponent->deselected.off(this);
	}

	m_owner->addedToMap.off(this);
}

void SpriteComponent::setCycleAnimation(const AnimationDescription& animationDescription)
{
	if (m_cycleAnimationDescription != &animationDescription)
	{
		m_cycleAnimationDescription = &animationDescription;
		if (m_currentAnimationDescription == nullptr)
		{
			playAnimation(*m_cycleAnimationDescription);
			m_sprite.setAnimated(m_isCycleAnimated);
		}
	}
}

bool SpriteComponent::setCycleAnimationByName(const std::string& animationName)
{
	const SpriteDescription& spriteDescription = getTemplate()->getSpriteDescription();
	const AnimationDescription* animationDescription = spriteDescription.getAnimationDescription(animationName);
	if (animationDescription != nullptr)
	{
		setCycleAnimation(*animationDescription);
		return true;
	}
	return false;
}

void SpriteComponent::clearCycleAnimation()
{
	if (m_cycleAnimationDescription != nullptr && m_currentAnimationDescription == nullptr)
	{
		m_sprite.setAnimated(false);
	}
	m_cycleAnimationDescription = nullptr;
}

void SpriteComponent::setCycleAnimated(bool isCycleAnimated)
{
	m_isCycleAnimated = isCycleAnimated;
	if (m_currentAnimationDescription == nullptr)
	{
		m_sprite.setAnimated(isCycleAnimated);
	}
}

void SpriteComponent::resetCycleAnimation()
{
	if (m_currentAnimationDescription == nullptr)
	{
		m_sprite.setColumn(0);
	}
}

void SpriteComponent::playAnimation(const AnimationDescription& animationDescription, int numLoops, bool preventBusy)
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
	if (&animationDescription != m_cycleAnimationDescription)
	{
		m_currentAnimationDescription = &animationDescription;
		m_preventBusy = preventBusy;
	}
	else
	{
		m_preventBusy = true;
	}
}

bool SpriteComponent::playAnimationByName(const std::string& animationName, int numLoops, bool preventBusy)
{
	const SpriteDescription& spriteDescription = getTemplate()->getSpriteDescription();
	const AnimationDescription* animationDescription = spriteDescription.getAnimationDescription(animationName);
	if (animationDescription != nullptr)
	{
		playAnimation(*animationDescription, numLoops, preventBusy);
		return true;
	}
	return false;
}

bool SpriteComponent::getAttachPoint(const std::string& attachPointName, flat::Vector3& attachPoint) const
{
	const SpriteDescription& spriteDescription = getTemplate()->getSpriteDescription();
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
	m_sprite.update(currentTime);

	if (!m_sprite.isAnimated())
	{
		m_currentAnimationDescription = nullptr;
		if (m_cycleAnimationDescription != nullptr)
		{
			playAnimation(*m_cycleAnimationDescription);
			m_sprite.setAnimated(m_isCycleAnimated);
		}
	}
}

bool SpriteComponent::isBusy() const
{
	return !m_preventBusy && m_currentAnimationDescription != nullptr;
}

#ifdef FLAT_DEBUG
void SpriteComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	debugDisplay.add2dAABB(m_owner->getAABB());

	// below: debug the owner's sprite's quadtree cell
	
	const map::Map* map = m_owner->getMap();
	const map::DisplayManager& mapDisplayManager = map->getDisplayManager();
	const flat::AABB2& quadtreeCellAABB = mapDisplayManager.getEntityCellAABB(m_owner);
	debugDisplay.add2dAABB(quadtreeCellAABB, flat::video::Color::RED);
	if (m_currentAnimationDescription != nullptr)
	{
		debugDisplay.add3dText(m_owner->getPosition(), m_currentAnimationDescription->getName()
			+ " " + std::to_string(m_sprite.getCurrentColumn() + 1) + "/" + std::to_string(m_currentAnimationDescription->getNumFrames()));
	}
	else if (m_cycleAnimationDescription != nullptr)
	{
		debugDisplay.add3dText(m_owner->getPosition(), m_cycleAnimationDescription->getName()
			+ " " + std::to_string(m_sprite.getCurrentColumn() + 1) + "/" + std::to_string(m_cycleAnimationDescription->getNumFrames()) + " (cycle)");
	}
}
#endif

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

bool SpriteComponent::addedToMap(Entity* entity, map::Map* map)
{
	const flat::Vector3& position = entity->getPosition();
	flat::Vector2 position2d(map->getTransform() * position);
	m_sprite.setPosition(position2d);
	return true;
}

} // sprite
} // component
} // entity
} // game



