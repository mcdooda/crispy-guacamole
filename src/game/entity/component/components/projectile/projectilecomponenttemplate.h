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
		void load(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath) override final;

		inline const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getCollidedCallback() const { return m_collidedCallback; }
		inline float getWeight() const { return m_weight; }
		inline float getSpeed() const { return m_speed; }
		inline bool getRotateSprite() const { return m_rotateSprite; }

	private:
		flat::lua::SharedLuaReference<LUA_TFUNCTION> m_collidedCallback;
		float m_weight;
		float m_speed;
		bool m_rotateSprite;
};

} // projectile
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_PROJECTILE_PROJECTILECOMPONENTTEMPLATE_H


