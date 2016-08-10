#ifndef GAME_ENTITY_COMPONENT_SPRITECOMPONENT_H
#define GAME_ENTITY_COMPONENT_SPRITECOMPONENT_H

#include <flat.h>
#include "component.h"

namespace game
{
namespace entity
{
namespace component
{

class SpriteComponent : public Component
{
	typedef Component Super;
	public:
		void setOwner(Entity* owner) override;
		void update(float currentTime, float elapsedTime) override;
		
		void updateHeading();
		void updatePosition();
		void draw(const flat::util::RenderSettings& renderSettings, const flat::geometry::Matrix4& viewMatrix) const;
		
		inline void setColumn(int column) { m_sprite.setColumn(column); }
		
	private:
		flat::util::AnimatedSprite m_sprite;
};

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_SPRITECOMPONENT_H



