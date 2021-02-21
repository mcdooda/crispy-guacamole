#ifndef GAME_ENTITY_COMPONENT_SELECTION_SELECTIONCOMPONENT_H
#define GAME_ENTITY_COMPONENT_SELECTION_SELECTIONCOMPONENT_H

#include <flat.h>
#include "selectioncomponenttemplate.h"
#include "../../component.h"

namespace game::entity::component::selection
{

class SelectionComponent : public ComponentImpl<SelectionComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "selection"; }
		inline static const char* getVisualName() { return "Selection"; }
		inline static ComponentUpdateType getUpdateType() { return ComponentUpdateType::NONE; }

		void init() override;
		void deinit() override;

		int addSelectedCallback(lua_State* L, int index);
		void removeSelectedCallback(int index);

		int addDeselectedCallback(lua_State* L, int index);
		void removeDeselectedCallback(int index);

		int addClickCallback(lua_State* L, int index);
		void removeClickCallback(int index);

	public:
		flat::Slot<> selected;
		flat::Slot<> deselected;
		flat::Slot<> click;
		flat::Slot<> mouseEntered;
		flat::Slot<> mouseLeft;

	private:
		flat::lua::SlotProxy<> m_selectedSlotProxy;
		flat::lua::SlotProxy<> m_deselectedSlotProxy;
		flat::lua::SlotProxy<> m_clickSlotProxy;
};

} // game::entity::component::selection

#endif // GAME_ENTITY_COMPONENT_SELECTION_SELECTIONCOMPONENT_H



