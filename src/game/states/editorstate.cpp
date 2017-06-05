#include <algorithm>
#include <iostream>
#include "editorstate.h"
#include "editor/tileeditormode.h"
#include "editor/entityeditormode.h"
#include "../game.h"
#include "../map/tile.h"
#include "../map/brush/spherebrush.h"
#include "../entity/component/components/behavior/behaviorcomponent.h"

#ifdef FLAT_DEBUG
#include "gamestate.h"
#endif

namespace game
{
namespace states
{

void EditorState::enter(Game& game)
{
	Super::enter(game);
	
#ifdef FLAT_DEBUG
	if (!m_isReloading)
	{
#endif
		setCameraZoom(0.5f);
#ifdef FLAT_DEBUG
	}
#endif

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
	const auto& time = game.time;
	m_map.updateEntities(time->getTime(), time->getFrameTime());

#ifdef FLAT_DEBUG
	const auto& keyboard = game.input->keyboard;

	if (keyboard->isJustPressed(K(F9)))
	{
		return reloadToState<GameState>(game);
	}
#endif

	Super::execute(game);
}

entity::component::ComponentFlags EditorState::getComponentsFilter() const
{
	return m_componentRegistry.getEditorComponentsFilter();
}

void EditorState::saveOnCtrlS(Game& game)
{
	auto& keyboard = game.input->keyboard;
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
	const auto& input = game.input;

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
			auto& mouse = input->mouse;
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


