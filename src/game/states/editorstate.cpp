#include <algorithm>
#include "editorstate.h"
#include "editor/tileeditormode.h"
#include "editor/entityeditormode.h"
#include "../game.h"
#include "../map/tile.h"
#include "../map/brush/spherebrush.h"
#include "../entity/component/behaviorcomponent.h"

namespace game
{
namespace states
{

void EditorState::enter(Game& game)
{
	Super::enter(game);
	
	setCameraZoom(0.5f);

	flat::lua::doFile(m_luaState, "data/editor/scripts/ui.lua");
}

void EditorState::execute(Game& game)
{
	FLAT_ASSERT(m_editorMode.get() != nullptr);

	updateBrush(game);
	displayBrush(game);
	applyBrush(game);
	saveOnCtrlS(game);

	flat::time::Time* time = game.time;
	m_map.updateEntities(time->getTime(), time->getFrameTime());
	
	Super::execute(game);
}

entity::component::ComponentFlags EditorState::getComponentsFilter() const
{
	return Super::getComponentsFilter() & ~entity::component::BehaviorComponent::getFlag();
}

void EditorState::saveOnCtrlS(Game& game)
{
	const flat::input::Keyboard* keyboard = game.input->keyboard;
	if (keyboard->isPressed(K(LCTRL)) && keyboard->isJustPressed(K(S)))
	{
		saveMap(game);
	}
}

void EditorState::updateBrush(Game& game)
{
	m_editorMode->updateBrushPosition();
	m_editorMode->updateBrushTiles();
}

void EditorState::displayBrush(Game& game)
{
	m_editorMode->displayBrush();
}

void EditorState::applyBrush(Game& game)
{
	const flat::input::Input* input = game.input;

	if (!input->keyboard->isPressed(K(LCTRL)))
	{
		m_editorMode->handleShortcuts();
	}

	if (!m_ui->isMouseOver())
	{
		const flat::input::Mouse* mouse = input->mouse;
		if (mouse->isPressed(M(LEFT)))
		{
			m_editorMode->applyBrushPrimaryEffect(mouse->isJustPressed(M(LEFT)));
		}
		else if (mouse->isPressed(M(RIGHT)))
		{
			m_editorMode->applyBrushSecondaryEffect(mouse->isJustPressed(M(RIGHT)));
		}
	}
}

} // states
} // game


