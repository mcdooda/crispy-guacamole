#include "selectioncomponent.h"
#include "../../../lua/entity.h"

namespace game
{
namespace entity
{
namespace component
{
namespace selection
{

void SelectionComponent::init()
{
	m_owner->setCanBeSelected(getTemplate()->canBeSelected());

	auto pushEntityCallback = [this](lua_State* L)
	{
		lua::pushEntity(L, m_owner);
	};
	m_selectedSlotProxy.init(&selected, pushEntityCallback);
	m_deselectedSlotProxy.init(&deselected, pushEntityCallback);
	m_clickSlotProxy.init(&click, pushEntityCallback);
}

void SelectionComponent::deinit()
{
	if (m_owner->isSelected())
	{
		m_owner->setSelected(false);
	}

	m_selectedSlotProxy.reset();
	m_deselectedSlotProxy.reset();
	m_clickSlotProxy.reset();
}

int SelectionComponent::addSelectedCallback(lua_State* L, int index)
{
	return m_selectedSlotProxy.addCallback(L, index);
}

void SelectionComponent::removeSelectedCallback(int index)
{
	m_selectedSlotProxy.removeCallback(index);
}

int SelectionComponent::addDeselectedCallback(lua_State* L, int index)
{
	return m_deselectedSlotProxy.addCallback(L, index);
}

void SelectionComponent::removeDeselectedCallback(int index)
{
	m_deselectedSlotProxy.removeCallback(index);
}

int SelectionComponent::addClickCallback(lua_State* L, int index)
{
	return m_clickSlotProxy.addCallback(L, index);
}

void SelectionComponent::removeClickCallback(int index)
{
	m_clickSlotProxy.removeCallback(index);
}

} // selection
} // component
} // entity
} // game


