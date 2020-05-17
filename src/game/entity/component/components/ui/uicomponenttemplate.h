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
		void load(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath) override final;

		inline flat::sharp::ui::WidgetFactory& getWidgetFactory() const { return *m_widgetFactory; }

		inline const flat::lua::UniqueLuaReference<LUA_TFUNCTION>& getAddedToMap() const { return m_addedToMap; }
		inline const flat::lua::UniqueLuaReference<LUA_TFUNCTION>& getRemovedFromMap() const { return m_removedFromMap; }
		inline const flat::lua::UniqueLuaReference<LUA_TFUNCTION>& getSelected() const { return m_selected; }
		inline const flat::lua::UniqueLuaReference<LUA_TFUNCTION>& getDeselected() const { return m_deselected; }

	private:
		flat::sharp::ui::WidgetFactory* m_widgetFactory; // give access to UiComponent

		flat::lua::UniqueLuaReference<LUA_TFUNCTION> m_addedToMap;
		flat::lua::UniqueLuaReference<LUA_TFUNCTION> m_removedFromMap;
		flat::lua::UniqueLuaReference<LUA_TFUNCTION> m_selected;
		flat::lua::UniqueLuaReference<LUA_TFUNCTION> m_deselected;
};

} // ui
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_UI_UICOMPONENTTEMPLATE_H


