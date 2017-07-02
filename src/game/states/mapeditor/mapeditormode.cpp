#include "mapeditormode.h"
#include "../mapeditorstate.h"
#include "../../game.h"

namespace game
{
namespace states
{
namespace editor
{

MapEditorMode::MapEditorMode(Game& game) :
	m_game(game)
{

}

MapEditorMode::~MapEditorMode()
{
	// do not use getMap() as the state may be being destroyed at the time
	map::Map& map = m_game.getStateMachine().getState()->as<states::MapEditorState>().getMap();

	map.eachTileIfExists([](map::Tile* tile)
	{
		tile->setColor(flat::video::Color::WHITE);
	});
}

void MapEditorMode::updateBrushPosition()
{
	const auto& keyboard = m_game.input->keyboard;
	m_brushPosition = getEditorState().getCursorMapPosition(m_game, m_brushOnTile);
	if (keyboard->isPressed(K(LSHIFT)))
	{
		m_brushPosition.x = std::round(m_brushPosition.x);
		m_brushPosition.y = std::round(m_brushPosition.y);
	}
}

void MapEditorMode::clearBrush() const
{
	auto clearTiles = [](map::Tile* tile, float effect)
	{
		tile->setColor(flat::video::Color::WHITE);
	};
	eachSelectedTile(clearTiles);
	eachBrushTile(clearTiles);
}

void MapEditorMode::displayBrush() const
{
	for (const map::brush::TileEffect& tileEffect : m_selectedTiles)
	{
		flat::video::Color color(1.f - tileEffect.effect, 1.f - tileEffect.effect, 1.f, 1.f);
		tileEffect.tile->setColor(color);
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

void MapEditorMode::eachSelectedTile(std::function<void(map::Tile*, float)> func) const
{
	const map::brush::TilesContainer& selectedTiles = !m_selectedTiles.empty() ? m_selectedTiles : m_brushTiles;
	for (const map::brush::TileEffect& tileEffect : selectedTiles)
	{
		if (tileEffect.effect > 0.f)
		{
			func(tileEffect.tile, tileEffect.effect);
		}
	}
}

void MapEditorMode::eachSelectedTileIfExists(std::function<void(map::Tile*, float)> func) const
{
	eachSelectedTile([func](map::Tile* tile, float effect)
	{
		if (tile->exists())
		{
			func(tile, effect);
		}
	});
}

void MapEditorMode::eachBrushTile(std::function<void(map::Tile*, float)> func) const
{
	for (const map::brush::TileEffect& tileEffect : m_brushTiles)
	{
		if (tileEffect.effect > 0.f)
		{
			func(tileEffect.tile, tileEffect.effect);
		}
	}
}

void MapEditorMode::eachBrushTileIfExists(std::function<void(map::Tile*, float)> func) const
{
	eachBrushTile([func](map::Tile* tile, float effect)
	{
		if (tile->exists())
		{
			func(tile, effect);
		}
	});
}

void MapEditorMode::clearSelectedTiles()
{
	for (const map::brush::TileEffect& tileEffect : m_selectedTiles)
	{
		tileEffect.tile->setColor(flat::video::Color::WHITE);
	}
	m_selectedTiles.clear();
}

states::MapEditorState& MapEditorMode::getEditorState() const
{
	return m_game.getStateMachine().getState()->to<states::MapEditorState>();
}

map::Map& MapEditorMode::getMap() const
{
	return getEditorState().getMap();
}

const flat::time::Clock& MapEditorMode::getClock() const
{
	return getEditorState().getClock();
}

} // editor
} // states
} // game


