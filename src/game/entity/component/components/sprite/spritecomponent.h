#ifndef GAME_ENTITY_COMPONENT_SPRITE_SPRITECOMPONENT_H
#define GAME_ENTITY_COMPONENT_SPRITE_SPRITECOMPONENT_H

#include <flat.h>
#include "spritecomponenttemplate.h"
#include "../../component.h"

namespace game
{
namespace entity
{
namespace component
{
namespace sprite
{

class SpriteComponent : public ComponentImpl<SpriteComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "sprite"; }

		void init() override;
		void deinit() override;

		void update(float currentTime, float elapsedTime) override;
		
		bool isBusy() const override;
		
		void playAnimation(const sprite::AnimationDescription& animationDescription, int numLoops = 1);
		bool playAnimationByName(const std::string& animationName, int numLoops = 1);
		bool setMoveAnimationByName(const std::string& moveAnimationName);
		bool setDefaultMoveAnimation();

		bool getAttachPoint(const std::string& attachPointName, flat::Vector3& attachPoint) const;
		
		inline const flat::render::Sprite& getSprite() const { return m_sprite; }
		
	private:
		bool headingChanged(float heading);
		bool positionChanged(const flat::Vector3& position);
		bool movementStarted();
		bool movementStopped();
		bool selected();
		bool deselected();
		
	private:
		flat::render::AnimatedSprite m_sprite;
		const sprite::AnimationDescription* m_moveAnimationDescription;
		const sprite::AnimationDescription* m_currentAnimationDescription;
		bool m_positionChanged : 1;
		bool m_headingChanged : 1;
		bool m_movementStarted : 1;
		bool m_movementStopped : 1;
};

} // sprite
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_SPRITE_SPRITECOMPONENT_H


