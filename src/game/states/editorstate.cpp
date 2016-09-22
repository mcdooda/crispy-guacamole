#include <algorithm>
#include "editorstate.h"
#include "../game.h"
#include "../map/tile.h"
#include "../map/brush/spherebrush.h"
#include "editor/tileeditormode.h"
#include "editor/entityeditormode.h"

namespace game
{
namespace states
{

void EditorState::enter(flat::state::Agent* agent)
{
	Super::enter(agent);
	Game* game = agent->to<Game>();
	
	setCameraZoom(0.5f);
	
	flat::lua::doFile(m_luaState, "data/editor/scripts/ui.lua");

	editor::EditorMode* editorMode = new editor::EntityEditorMode(game, this);
	m_editorMode.reset(editorMode);
}

void EditorState::execute(flat::state::Agent* agent)
{
	Game* game = agent->to<Game>();

	updateBrush(game);
	displayBrush(game);
	applyBrush(game);
	saveOnCtrlS(game);
	
	Super::execute(agent);
}

void EditorState::saveOnCtrlS(Game* game)
{
	const flat::input::Keyboard* keyboard = game->input->keyboard;
	if (keyboard->isPressed(K(LCTRL)) && keyboard->isJustPressed(K(S)))
	{
		saveMap(game);
	}
}

void EditorState::updateBrush(Game* game)
{
	m_editorMode->updateBrushPosition();
	m_editorMode->updateBrushTiles();
}

void EditorState::displayBrush(Game* game)
{
	m_editorMode->displayBrush();
}

void EditorState::applyBrush(Game* game)
{
	const flat::input::Input* input = game->input;

	if (!input->keyboard->isPressed(K(LCTRL)))
	{
		m_editorMode->handleShortcuts();
	}

	if (input->mouse->isPressed(M(LEFT)))
	{
		m_editorMode->applyBrush();
	}
}

} // states
} // game


