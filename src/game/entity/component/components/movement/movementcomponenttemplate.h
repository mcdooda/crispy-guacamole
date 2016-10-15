#ifndef GAME_ENTITY_COMPONENT_MOVEMENTCOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_MOVEMENTCOMPONENTTEMPLATE_H

#include "../../componenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{

class MovementComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::string& entityTemplatePath) override;

		inline float getJumpHeight(float t) const { return (-m_weight / 2.f) * t * t + m_jumpForce * t; }

	private:
		float m_radius;
		float m_speed;
		float m_jumpForce;
		float m_weight;

		float m_jumpMaxHeight;
		float m_jumpDuration;
		float m_jumpDistance;
};

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_MOVEMENTCOMPONENTTEMPLATE_H


