#ifndef GAME_ENTITY_COMPONENT_PLAYERCONTROLLER_PLAYERCONTROLLERCOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_PLAYERCONTROLLER_PLAYERCONTROLLERCOMPONENTTEMPLATE_H

#include "../../componenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace playercontroller
{

class PlayerControllerComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::string& entityTemplatePath) override final;

		inline const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getUpdate() const { return m_update; }

	private:
		flat::lua::SharedLuaReference<LUA_TFUNCTION> m_update;
};

} // playercontroller
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_PLAYERCONTROLLER_PLAYERCONTROLLERCOMPONENTTEMPLATE_H


