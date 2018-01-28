#ifndef GAME_ENTITY_COMPONENT_UI_UICOMPONENT_H
#define GAME_ENTITY_COMPONENT_UI_UICOMPONENT_H

#include <flat.h>
#include "uicomponenttemplate.h"
#include "../../component.h"

namespace game
{
namespace entity
{
namespace component
{
namespace ui
{

class UiComponent : public ComponentImpl<UiComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "ui"; }
		inline static bool enableInMapEditor() { return false; }

		void init(lua_State* L) override;
		void deinit(lua_State* L) override;

		void update(lua_State* L, float currentTime, float elapsedTime) override;

		inline void setWidgetOffset(const flat::Vector2& widgetOffset) { m_widgetOffset = widgetOffset; }
		inline void setWidgetVisible(bool widgetVisible) { m_widgetVisible = widgetVisible; }

	private:
		bool addedToMap(lua_State* L, Entity* entity, map::Map* map);
		bool removedFromMap(lua_State* L, Entity* entity);
		bool selected();
		bool deselected();

		flat::sharp::ui::WidgetFactory& getWidgetFactory() const;

		void triggerCallback(lua_State* L, const flat::lua::UniqueLuaReference<LUA_TFUNCTION>& function);

	private:
		std::shared_ptr<flat::sharp::ui::Widget> m_widget;
		flat::Vector2 m_widgetOffset;
		const flat::video::View* m_gameView;
		bool m_widgetVisible : 1;
		bool m_addedToMap : 1;
		bool m_removedFromMap : 1;
		bool m_selected : 1;
		bool m_deselected : 1;
};

} // ui
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_UI_UICOMPONENT_H



