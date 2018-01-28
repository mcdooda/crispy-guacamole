#include "uicomponent.h"
#include "../../../lua/entity.h"
#include "../../../../map/map.h"
#include "../../../../game.h"
#include "../../../../states/basemapstate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace ui
{

void UiComponent::init(lua_State* L)
{
	m_widgetVisible = true;
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

	Game& game = flat::lua::getFlatAs<Game>(L);
	states::BaseMapState& baseMapState = game.getStateMachine().getState()->to<states::BaseMapState>();
	m_gameView = &baseMapState.getGameView();
}

void UiComponent::deinit(lua_State* L)
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

	m_gameView = nullptr;
}

void UiComponent::update(lua_State* L, float currentTime, float elapsedTime)
{
	const UiComponentTemplate* uiComponentTemplate = getTemplate();

	if (m_addedToMap)
	{
		triggerCallback(L, uiComponentTemplate->getAddedToMap());
		m_addedToMap = false;
	}
	else if (m_removedFromMap)
	{
		triggerCallback(L, uiComponentTemplate->getRemovedFromMap());
		m_removedFromMap = false;
	}

	// update widget position
	flat::sharp::ui::Widget* widget = m_widget.get();
	if (m_widgetVisible)
	{
		flat::AABB2 screenAABB;
		m_gameView->getScreenAABB(screenAABB);

		const flat::AABB2& spriteAABB = m_owner->getAABB();

		if (flat::AABB2::overlap(screenAABB, spriteAABB))
		{
			widget->setVisible(true);
			flat::Vector2 position2d = m_gameView->getWindowPosition(m_owner->getSprite().getPosition());
			flat::sharp::ui::Widget::Position widgetPosition = position2d + m_widgetOffset;
			widget->setPosition(widgetPosition);
		}
		else
		{
			widget->setVisible(false);
		}
	}
	else
	{
		widget->setVisible(false);
	}

	if (m_selected)
	{
		triggerCallback(L, uiComponentTemplate->getSelected());
		m_selected = false;
	}
	else if (m_deselected)
	{
		triggerCallback(L, uiComponentTemplate->getDeselected());
		m_deselected = false;
	}
}

bool UiComponent::addedToMap(lua_State* L, Entity* entity, map::Map* map)
{
	m_addedToMap = true;
	return true;
}

bool UiComponent::removedFromMap(lua_State* L, Entity* entity)
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

void UiComponent::triggerCallback(lua_State* L, const flat::lua::UniqueLuaReference<LUA_TFUNCTION>& function)
{
	if (function.isEmpty())
	{
		return;
	}
	
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



