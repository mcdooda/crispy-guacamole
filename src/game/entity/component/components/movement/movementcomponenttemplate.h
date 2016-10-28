#ifndef GAME_ENTITY_COMPONENT_MOVEMENT_MOVEMENTCOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_MOVEMENT_MOVEMENTCOMPONENTTEMPLATE_H

#include "../../componenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace movement
{

class MovementComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::string& entityTemplatePath) override final;

		inline float getSpeed() const { return m_speed; }
		inline float getJumpForce() const { return m_jumpForce; }
		inline float getWeight() const { return m_weight; }
		
		inline float getJumpHeight(float t) const { return (-m_weight / 2.f) * t * t + m_jumpForce * t; }
		inline float getJumpMaxHeight() const { return m_jumpMaxHeight; }
		inline float getJumpDuration() const { return m_jumpDuration; }
		inline float getJumpDistance() const { return m_jumpDistance; }

	private:
		float m_speed;
		float m_jumpForce;
		float m_weight;

		float m_jumpMaxHeight;
		float m_jumpDuration;
		float m_jumpDistance;
};

} // movement
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_MOVEMENT_MOVEMENTCOMPONENTTEMPLATE_H


