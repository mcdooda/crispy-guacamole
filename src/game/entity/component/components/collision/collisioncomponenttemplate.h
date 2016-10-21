#ifndef GAME_ENTITY_COMPONENT_COLLISIONCOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_COLLISIONCOMPONENTTEMPLATE_H

#include "../../componenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{

class CollisionComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::string& entityTemplatePath) override final;

		inline float getRadius() const { return m_radius; }

	private:
		float m_radius;
};

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_COLLISIONCOMPONENTTEMPLATE_H


