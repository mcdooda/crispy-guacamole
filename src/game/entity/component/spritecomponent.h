#ifndef GAME_ENTITY_COMPONENT_SPRITECOMPONENT_H
#define GAME_ENTITY_COMPONENT_SPRITECOMPONENT_H

#include <flat.h>
#include "component.h"

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

class SpriteComponent : public Component
{
	typedef Component Super;
	public:
		void setOwner(Entity* owner) override;
		void update(float currentTime, float elapsedTime) override;
		
		bool isBusy() const override;
		
		void playAnimation(const sprite::AnimationDescription& animationDescription, int numLoops = 1);
		
		void draw(const flat::util::RenderSettings& renderSettings, const flat::Matrix4& viewMatrix) const;
		
	private:
		void headingChanged(float heading);
		void positionChanged(const flat::Vector3& position);
		void movementStarted();
		void movementStopped();
		
	private:
		flat::util::AnimatedSprite m_sprite;
};

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_SPRITECOMPONENT_H



