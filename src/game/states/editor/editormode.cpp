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

void EditorMode::clearBrush() const
{
	auto clearTiles = [](map::Tile* tile, float effect)
	{
		tile->setColor(flat::video::Color::WHITE);
	};
	eachSelectedTile(clearTiles);
	eachBrushTile(clearTiles);
}

void EditorMode::displayBrush() const
{
	eachSelectedTile([](map::Tile* tile, float effect)
	{
		flat::video::Color color(1.f - effect, 1.f - effect, 1.f, 1.f);
		tile->setColor(color);
	});
	eachBrushTile([](map::Tile* tile, float effect)
	{
		const flat::video::Color& currentColor = tile->getColor();
		flat::video::Color color(currentColor.r, (1.f - effect) * currentColor.g, (1.f - effect) * currentColor.b, 1.f);
		tile->setColor(color);
	});
}

void EditorMode::eachSelectedTile(std::function<void(map::Tile*, float)> func) const
{
	for (const std::pair<map::Tile*, float>& tile : m_selectedTiles)
	{
		const float effect = tile.second;
		if (effect > 0.f)
		{
			func(tile.first, effect);
		}
	}
}

void EditorMode::eachBrushTile(std::function<void(map::Tile*, float)> func) const
{
	for (const std::pair<map::Tile*, float>& tile : m_brushTiles)
	{
		const float effect = tile.second;
		if (effect > 0.f)
		{
			func(tile.first, effect);
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


