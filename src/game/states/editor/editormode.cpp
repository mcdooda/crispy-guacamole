#include "editormode.h"
#include "../editorstate.h"
#include "../../game.h"

namespace game
{
namespace states
{
namespace editor
{

EditorMode::EditorMode(Game& game) :
	m_game(game)
{

}

EditorMode::~EditorMode()
{

}

void EditorMode::updateBrushPosition()
{
	const flat::input::Keyboard* keyboard = m_game.input->keyboard;
	m_brushPosition = getEditorState().getCursorMapPosition(m_game);
	if (keyboard->isPressed(K(LSHIFT)))
	{
		m_brushPosition.x = std::round(m_brushPosition.x);
		m_brushPosition.y = std::round(m_brushPosition.y);
	}
}

void EditorMode::eachBrushTileIfExists(std::function<void(map::Tile*, float)> func) const
{
	map::brush::Brush* brush = m_brush.get();
	FLAT_ASSERT(brush != nullptr);
	for (map::Tile* tile : m_brushTiles)
	{
		if (tile->exists())
		{
			float effect = brush->getTileEffect(m_brushPosition, tile);
			if (effect > 0.f)
			{
				func(tile, effect);
			}
		}
	}
}

void EditorMode::eachBrushTile(std::function<void(map::Tile*, float)> func) const
{
	map::brush::Brush* brush = m_brush.get();
	FLAT_ASSERT(brush != nullptr);
	for (map::Tile* tile : m_brushTiles)
	{
		float effect = brush->getTileEffect(m_brushPosition, tile);
		if (effect > 0.f)
		{
			func(tile, effect);
		}
	}
}

states::EditorState& EditorMode::getEditorState() const
{
	return m_game.getStateMachine().getState()->to<states::EditorState>();
}

map::Map& EditorMode::getMap() const
{
	return getEditorState().getMap();
}

} // editor
} // states
} // game


