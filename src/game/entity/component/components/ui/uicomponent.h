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
		inline static bool enableInEditor() { return false; }

		void init() override;
		void deinit() override;

		void update(float currentTime, float elapsedTime) override;

	private:
		bool addedToMap(Entity* entity, map::Map* map);
		bool removedFromMap(Entity* entity);
		bool selected();
		bool deselected();

		flat::sharp::ui::WidgetFactory& getWidgetFactory() const;
		flat::sharp::ui::RootWidget* getRootWidget() const;

		void triggerCallback(const flat::lua::SharedLuaReference<LUA_TFUNCTION>& function);

	private:
		std::shared_ptr<flat::sharp::ui::Widget> m_widget;
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


