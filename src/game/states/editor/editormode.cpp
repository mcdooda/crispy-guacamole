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
	// do not use getMap() as the state may be being destroyed at the time
	map::Map& map = m_game.getStateMachine().getState()->as<states::EditorState>().getMap();

	map.eachTileIfExists([](map::Tile* tile)
	{
		tile->setColor(flat::video::Color::WHITE);
	});
}

void EditorMode::updateBrushPosition()
{
	const auto& keyboard = m_game.input->keyboard;
	m_brushPosition = getEditorState().getCursorMapPosition(m_game, m_brushOnTile);
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
	for (const std::pair<map::Tile*, float>& pair : m_selectedTiles)
	{
		map::Tile* tile = pair.first;
		float effect = pair.second;
		flat::video::Color color(1.f - effect, 1.f - effect, 1.f, 1.f);
		tile->setColor(color);
	}

	if (!getEditorState().isMouseOverUi(m_game))
	{
		eachBrushTile([](map::Tile* tile, float effect)
		{
			const flat::video::Color& currentColor = tile->getColor();
			flat::video::Color color(currentColor.r, (1.f - effect) * currentColor.g, (1.f - effect) * currentColor.b, 1.f);
			tile->setColor(color);
		});
	}
}

void EditorMode::eachSelectedTile(std::function<void(map::Tile*, float)> func) const
{
	std::map<map::Tile*, float> selectedTiles = !m_selectedTiles.empty() ? m_selectedTiles : m_brushTiles;
	for (const std::pair<map::Tile*, float>& tile : selectedTiles)
	{
		const float effect = tile.second;
		if (effect > 0.f)
		{
			func(tile.first, effect);
		}
	}
}

void EditorMode::eachSelectedTileIfExists(std::function<void(map::Tile*, float)> func) const
{
	eachSelectedTile([func](map::Tile* tile, float effect)
	{
		if (tile->exists())
		{
			func(tile, effect);
		}
	});
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

void EditorMode::eachBrushTileIfExists(std::function<void(map::Tile*, float)> func) const
{
	eachBrushTile([func](map::Tile* tile, float effect)
	{
		if (tile->exists())
		{
			func(tile, effect);
		}
	});
}

void EditorMode::clearSelectedTiles()
{
	for (const std::pair<map::Tile*, float>& pair : m_selectedTiles)
	{
		pair.first->setColor(flat::video::Color::WHITE);
	}
	m_selectedTiles.clear();
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


