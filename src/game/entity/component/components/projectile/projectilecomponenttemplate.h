#ifndef GAME_ENTITY_COMPONENT_PROJECTILE_PROJECTILECOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_PROJECTILE_PROJECTILECOMPONENTTEMPLATE_H

#include "../../componenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace projectile
{

class ProjectileComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::string& entityTemplatePath) override final;

		inline float getWeight() const { return m_weight; }
		inline float getSpeed() const { return m_speed; }
		inline const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getCollidedCallback() const { return m_collidedCallback; }

	private:
		float m_weight;
		float m_speed;
		flat::lua::SharedLuaReference<LUA_TFUNCTION> m_collidedCallback;
};

} // projectile
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_PROJECTILE_PROJECTILECOMPONENTTEMPLATE_H


