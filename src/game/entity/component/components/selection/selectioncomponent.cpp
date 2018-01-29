#include "selectioncomponent.h"

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
	m_selected = false;
}

void SelectionComponent::deinit()
{
	if (m_selected)
	{
		setSelected(false);
	}
}

void SelectionComponent::setSelected(bool selected)
{
	FLAT_ASSERT(selected != m_selected); // we want to avoid triggering slots again if the selected state did not change
	m_selected = selected;
	if (selected)
	{
		this->selected();
	}
	else
	{
		deselected();
	}
}

} // selection
} // component
} // entity
} // game


