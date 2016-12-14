#ifndef GAME_ENTITY_COMPONENT_ATTACK_ATTACKCOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_ATTACK_ATTACKCOMPONENTTEMPLATE_H

#include <flat.h>
#include "../../componenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace attack
{

class AttackComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::string& entityTemplatePath) override final;

		inline const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getAttackFunc() const { return m_attackFunc; }
		inline float getAttackRange() const { return m_attackRange; }
		inline float getAttackCooldown() const { return m_attackCooldown; }
		inline bool getAutoAttack() const { return m_autoAttack; }
		inline bool getMoveDuringAttack() const { return m_moveDuringAttack; }

	private:
		flat::lua::SharedLuaReference<LUA_TFUNCTION> m_attackFunc;
		float m_attackRange;
		float m_attackCooldown;
		bool m_autoAttack : 1;
		bool m_moveDuringAttack : 1;
};

} // attack
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_ATTACK_ATTACKCOMPONENTTEMPLATE_H


