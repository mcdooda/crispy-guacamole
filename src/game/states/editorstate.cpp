#include <algorithm>
#include <iostream>
#include "editorstate.h"
#include "editor/tileeditormode.h"
#include "editor/entityeditormode.h"
#include "../game.h"
#include "../map/tile.h"
#include "../map/brush/spherebrush.h"
#include "../entity/component/components/behavior/behaviorcomponent.h"

namespace game
{
namespace states
{

void EditorState::enter(Game& game)
{
	Super::enter(game);
	
	setCameraZoom(0.5f);

	game.lua->doFile("data/editor/scripts/ui.lua");
}

void EditorState::execute(Game& game)
{
	FLAT_ASSERT(m_editorMode.get() != nullptr);

	clearBrush(game);
	updateBrush(game);
	displayBrush(game);
	applyBrush(game);
	saveOnCtrlS(game);

	despawnEntities();
	flat::time::Time* time = game.time;
	m_map.updateEntities(time->getTime(), time->getFrameTime());
	m_mapDisplayManager.updateEntities();

	Super::execute(game);
}

entity::component::ComponentFlags EditorState::getComponentsFilter() const
{
	return m_componentRegistry.getEditorComponentsFilter();
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

void EditorState::clearBrush(Game& game)
{
	m_editorMode->clearBrush();
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

	if (isMouseOverUi(game))
	{
		clearMouseOverEntity();
	}
	else
	{
		updateMouseOverEntity(game);
		const bool canSelectEntities = m_editorMode->canSelectEntities();
		if (!canSelectEntities || (canSelectEntities && !updateSelectionWidget(game)))
		{
			const flat::input::Mouse* mouse = input->mouse;
			if ((!canSelectEntities && mouse->isPressed(M(LEFT))) || mouse->isJustReleased(M(LEFT)))
			{
				m_editorMode->applyBrushPrimaryEffect(mouse->isJustPressed(M(LEFT)));
			}
			else if (mouse->isPressed(M(RIGHT)))
			{
				m_editorMode->applyBrushSecondaryEffect(mouse->isJustPressed(M(RIGHT)));
			}
		}
	}
}

} // states
} // game


