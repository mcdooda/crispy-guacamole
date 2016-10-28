#ifndef GAME_ENTITY_COMPONENT_COLLISIONCOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_COLLISIONCOMPONENTTEMPLATE_H

#include "collisionbox.h"
#include "../../componenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace collision
{

class CollisionComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::string& entityTemplatePath) override final;

		const CollisionBox& getCollisionBox() const { return m_collisionBox; }

		inline float getRadius() const { return m_collisionBox.getRadius(); }

	private:
		CollisionBox m_collisionBox;
};

} // collision
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_COLLISIONCOMPONENTTEMPLATE_H


