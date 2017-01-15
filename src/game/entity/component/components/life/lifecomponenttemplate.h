#ifndef GAME_ENTITY_COMPONENT_LIFE_LIFECOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_LIFE_LIFECOMPONENTTEMPLATE_H

#include <flat.h>
#include "../../componenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace life
{

class LifeComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::string& entityTemplatePath) override final;

		inline const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getSpawnFunc() const { return m_spawnFunc; }
		inline const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getDespawnFunc() const { return m_despawnFunc; }
		inline int getMaxHealth() const { return m_maxHealth; }

	private:
		flat::lua::SharedLuaReference<LUA_TFUNCTION> m_spawnFunc;
		flat::lua::SharedLuaReference<LUA_TFUNCTION> m_despawnFunc;
		int m_maxHealth;
};

} // life
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_LIFE_LIFECOMPONENTTEMPLATE_H


