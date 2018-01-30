#ifndef GAME_ENTITY_COMPONENT_SELECTION_SELECTIONCOMPONENT_H
#define GAME_ENTITY_COMPONENT_SELECTION_SELECTIONCOMPONENT_H

#include <flat.h>
#include "selectioncomponenttemplate.h"
#include "../../component.h"

namespace game
{
namespace entity
{
namespace component
{
namespace selection
{

class SelectionComponent : public ComponentImpl<SelectionComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "selection"; }

		void init() override;
		void deinit() override;

		void setSelected(bool selected);
		inline bool isSelected() const { return m_selected; }

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

	private:
		flat::lua::SlotProxy<> m_selectedSlotProxy;
		flat::lua::SlotProxy<> m_deselectedSlotProxy;
		flat::lua::SlotProxy<> m_clickSlotProxy;
		bool m_selected;
};

} // selection
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_SELECTION_SELECTIONCOMPONENT_H



