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

	public:
		flat::Slot<> selected;
		flat::Slot<> deselected;

	private:
		bool m_selected;
};

} // selection
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_SELECTION_SELECTIONCOMPONENT_H



