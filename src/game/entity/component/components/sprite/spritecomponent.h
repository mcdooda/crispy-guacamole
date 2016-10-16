#ifndef GAME_ENTITY_COMPONENT_SPRITECOMPONENT_H
#define GAME_ENTITY_COMPONENT_SPRITECOMPONENT_H

#include <flat.h>
#include "spritecomponenttemplate.h"
#include "../../component.h"

namespace game
{
namespace entity
{
namespace sprite
{
class AnimationDescription;
}
namespace component
{

class SpriteComponent : public ComponentImpl<SpriteComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "sprite"; }

		void init() override;

		void update(float currentTime, float elapsedTime) override;
		
		bool isBusy() const override;
		
		void playAnimation(const sprite::AnimationDescription& animationDescription, int numLoops = 1);
		
		inline const flat::util::Sprite& getSprite() const { return m_sprite; }
		
	private:
		void headingChanged(float heading);
		void positionChanged(const flat::Vector3& position);
		void movementStarted();
		void movementStopped();
		
	private:
		flat::util::AnimatedSprite m_sprite;
		bool m_positionChanged : 1;
		bool m_headingChanged : 1;
		bool m_movementStarted : 1;
		bool m_movementStopped : 1;
};

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_SPRITECOMPONENT_H



