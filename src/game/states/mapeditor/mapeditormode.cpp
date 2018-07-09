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
}

void MapEditorMode::enter(MapEditorState& mapEditorState)
{
}

void MapEditorMode::execute(MapEditorState& mapEditorState)
{
	clearBrush(mapEditorState);
	updateBrushPosition(mapEditorState);
	updateBrushTiles(mapEditorState);
	displayBrush(mapEditorState);
	applyBrush(mapEditorState);
}

void MapEditorMode::exit(MapEditorState& mapEditorState)
{
	map::Map& map = mapEditorState.getMap();

	map.eachTileIfExists([](map::Tile* tile)
	{
		tile->setColor(flat::video::Color::WHITE);
	});
}

void MapEditorMode::updateBrushPosition(MapEditorState& mapEditorState)
{
	const auto& keyboard = mapEditorState.m_gameInputContext->getKeyboardInputContext();
	m_brushPosition = mapEditorState.getCursorMapPosition(m_game, m_brushOnTile);
	if (keyboard.isPressed(K(LSHIFT)))
	{
		m_brushPosition.x = std::round(m_brushPosition.x);
		m_brushPosition.y = std::round(m_brushPosition.y);
	}
}

void MapEditorMode::clearBrush(MapEditorState& mapEditorState) const
{
	auto clearTiles = [](map::Tile* tile, float effect)
	{
		tile->setColor(flat::video::Color::WHITE);
	};
	eachSelectedTile(clearTiles);
	eachBrushTile(clearTiles);
}

void MapEditorMode::displayBrush(MapEditorState& mapEditorState) const
{
	for (const map::brush::TileEffect& tileEffect : m_selectedTiles)
	{
		flat::video::Color color(1.f - tileEffect.effect, 1.f - tileEffect.effect, 1.f, 1.f);
		tileEffect.tile->setColor(color);
	}

	if (!mapEditorState.isMouseOverUi(m_game))
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

void MapEditorMode::applyBrush(MapEditorState& mapEditorState)
{
	const auto& keyboard = mapEditorState.m_gameInputContext->getKeyboardInputContext();

	if (!keyboard.isPressed(K(LCTRL)))
	{
		handleShortcuts(mapEditorState);
	}

	if (mapEditorState.isMouseOverUi(m_game))
	{
		mapEditorState.clearMouseOverEntity();
	}
	else
	{
		mapEditorState.updateMouseOverEntity(m_game);
		const bool modeCanSelectEntities = canSelectEntities();
		if (!modeCanSelectEntities || (modeCanSelectEntities && !mapEditorState.updateSelectionWidget(m_game)))
		{
			const auto& mouse = mapEditorState.m_gameInputContext->getMouseInputContext();
			if ((!modeCanSelectEntities && mouse.isPressed(M(LEFT))) || mouse.isJustReleased(M(LEFT)))
			{
				applyBrushPrimaryEffect(mapEditorState, mouse.isJustPressed(M(LEFT)));
			}
			else if (mouse.isPressed(M(RIGHT)))
			{
				applyBrushSecondaryEffect(mapEditorState, mouse.isJustPressed(M(RIGHT)));
			}
		}
	}
}

} // editor
} // states
} // game


