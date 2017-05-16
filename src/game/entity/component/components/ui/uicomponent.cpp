#include "uicomponent.h"
#include "../../../lua/entity.h"

namespace game
{
namespace entity
{
namespace component
{
namespace ui
{

void UiComponent::init()
{
	m_addedToMap = false;
	m_removedFromMap = false;
	m_selected = false;
	m_deselected = false;

	flat::sharp::ui::WidgetFactory& widgetFactory = getWidgetFactory();
	m_widget = widgetFactory.makeColumnFlow();
	m_widget->setPositionPolicy(flat::sharp::ui::Widget::PositionPolicy::BOTTOM_LEFT);

	m_owner->addedToMap.on(this, &UiComponent::addedToMap);
	m_owner->removedFromMap.on(this, &UiComponent::removedFromMap);

	m_owner->selected.on(this, &UiComponent::selected);
	m_owner->deselected.on(this, &UiComponent::deselected);
}

void UiComponent::deinit()
{
	flat::sharp::ui::Widget* widget = m_widget.get();
	if (widget != nullptr)
	{
		if (!widget->getParent().expired())
		{
			widget->removeFromParent();
		}
		m_widget.reset();
	}

	m_owner->addedToMap.off(this);
	m_owner->removedFromMap.off(this);

	m_owner->positionChanged.off(this);

	m_owner->selected.off(this);
	m_owner->deselected.off(this);
}

void UiComponent::update(float currentTime, float elapsedTime)
{
	if (m_addedToMap)
	{
		triggerCallback(getTemplate()->getAddedToMap());
		m_addedToMap = false;
	}
	else if (m_removedFromMap)
	{
		triggerCallback(getTemplate()->getRemovedFromMap());
		m_removedFromMap = false;
	}

	triggerCallback(getTemplate()->getUpdate());

	if (m_selected)
	{
		triggerCallback(getTemplate()->getSelected());
		m_selected = false;
	}
	else if (m_deselected)
	{
		triggerCallback(getTemplate()->getDeselected());
		m_deselected = false;
	}
}

bool UiComponent::addedToMap(Entity * entity, map::Map * map)
{
	m_addedToMap = true;
	return true;
}

bool UiComponent::removedFromMap(Entity * entity)
{
	m_removedFromMap = true;
	return true;
}

bool UiComponent::selected()
{
	m_selected = true;
	return true;
}

bool UiComponent::deselected()
{
	m_deselected = true;
	return true;
}

flat::sharp::ui::WidgetFactory& UiComponent::getWidgetFactory() const
{
	return getTemplate()->getWidgetFactory();
}

void UiComponent::triggerCallback(const flat::lua::SharedLuaReference<LUA_TFUNCTION>& function)
{
	if (function.isEmpty())
	{
		return;
	}
	
	lua_State* L = function.getLuaState();
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		function.push(L);
		lua::pushEntity(L, m_owner);
		flat::sharp::ui::lua::pushWidget(L, m_widget);
		lua_call(L, 2, 0);
	}
}

} // ui
} // component
} // entity
} // game



