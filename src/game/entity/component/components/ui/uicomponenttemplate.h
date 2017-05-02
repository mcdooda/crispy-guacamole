#ifndef GAME_ENTITY_COMPONENT_UI_UICOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_UI_UICOMPONENTTEMPLATE_H

#include "../../componenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace ui
{

class UiComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::string& entityTemplatePath) override final;

		inline flat::sharp::ui::Ui& getUi() const { return *m_ui; }

		inline const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getAddedToMap() const { return m_addedToMap; }
		inline const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getRemovedFromMap() const { return m_removedFromMap; }
		inline const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getUpdate() const { return m_update; }
		inline const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getSelected() const { return m_selected; }
		inline const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getDeselected() const { return m_deselected; }

	private:
		flat::sharp::ui::Ui* m_ui; // give access to UiComponent

		flat::lua::SharedLuaReference<LUA_TFUNCTION> m_addedToMap;
		flat::lua::SharedLuaReference<LUA_TFUNCTION> m_removedFromMap;
		flat::lua::SharedLuaReference<LUA_TFUNCTION> m_update;
		flat::lua::SharedLuaReference<LUA_TFUNCTION> m_selected;
		flat::lua::SharedLuaReference<LUA_TFUNCTION> m_deselected;
};

} // ui
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_UI_UICOMPONENTTEMPLATE_H

