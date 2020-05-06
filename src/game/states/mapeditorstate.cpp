#include <algorithm>
#include <iostream>
#include <memory>
#include "mapeditorstate.h"
#include "mapeditor/tilemapeditormode.h"
#include "../game.h"
#include "../map/tile.h"
#include "../map/brush/spherebrush.h"
#include "../entity/component/components/behavior/behaviorcomponent.h"

#ifdef FLAT_DEBUG
// needed for reloadToState<GameState>
#include "gamestate.h"
#endif

namespace game
{
namespace states
{

void MapEditorState::enter(Game& game)
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

	getStateMachine().setState(std::make_unique<editor::TileMapEditorMode>(game));

	game.lua->doFile("data/editor/scripts/init.lua");
}

void MapEditorState::execute(Game& game)
{
	getStateMachine().update();

	saveOnCtrlS(game);

	updateEntities();
	updateMap();

#ifdef FLAT_DEBUG
	const auto& keyboard = game.input->keyboard;

	if (keyboard->isJustPressed(K(F7)))
	{
		return reloadToState<GameState>(game);
	}
#endif

	Super::execute(game);
}

editor::MapEditorMode* MapEditorState::getEditorMode()
{
	flat::state::State* mapEditorMode = getStateMachine().getState();
	if (mapEditorMode != nullptr)
	{
		return mapEditorMode->toP<editor::MapEditorMode>();
	}
	return nullptr;
}

void MapEditorState::setEditorMode(std::unique_ptr<editor::MapEditorMode>&& editorMode)
{
	getStateMachine().setState(std::move(editorMode));
}

void MapEditorState::draw(game::Game& game)
{
	const bool mouseOverUi = isMouseOverUi(game);
	if (!mouseOverUi)
	{
		getEditorMode()->preDraw(game);
	}
	Super::draw(game);
	if (!mouseOverUi)
	{
		getEditorMode()->postDraw(game);
	}
}

entity::component::ComponentFlags MapEditorState::getComponentsFilter() const
{
	return m_componentRegistry.getMapEditorComponentsFilter();
}

void MapEditorState::saveOnCtrlS(Game& game)
{
	auto& keyboard = game.input->keyboard;
	if (keyboard->isPressed(K(LCTRL)) && keyboard->isJustPressed(K(S)))
	{
		saveMap(game);
		game.notify->success(std::string("Map ") + game.mapPath + " saved successfully");
	}
}

} // states
} // game


