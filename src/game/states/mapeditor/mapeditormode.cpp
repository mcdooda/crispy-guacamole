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

	map.eachTile([&map](map::TileIndex tileIndex)
	{
		map.setTileColor(tileIndex, flat::video::Color::WHITE);
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
	map::Map& map = mapEditorState.getMap();
	auto clearTiles = [&map](map::TileIndex tileIndex, float effect)
	{
		map.setTileColor(tileIndex, flat::video::Color::WHITE);
	};
	eachSelectedTile(clearTiles);
	eachBrushTile(clearTiles);
}

void MapEditorMode::displayBrush(MapEditorState& mapEditorState) const
{
	for (const map::brush::TileEffect& tileEffect : m_selectedTiles)
	{
		flat::video::Color color(1.f - tileEffect.effect, 1.f - tileEffect.effect, 1.f, 1.f);
		mapEditorState.getMap().setTileColor(tileEffect.tileIndex, color);
	}

	if (!mapEditorState.isMouseOverUi(m_game))
	{
		map::Map& map = mapEditorState.getMap();
		eachBrushTile([&map](map::TileIndex tileIndex, float effect)
		{
			const flat::video::Color& currentColor = map.getTileColor(tileIndex);
			flat::video::Color color(currentColor.r, (1.f - effect) * currentColor.g, (1.f - effect) * currentColor.b, 1.f);
			map.setTileColor(tileIndex, color);
		});
	}
}

void MapEditorMode::eachSelectedTile(std::function<void(map::TileIndex, float)> func) const
{
	const map::brush::TilesContainer& selectedTiles = !m_selectedTiles.empty() ? m_selectedTiles : m_brushTiles;
	for (const map::brush::TileEffect& tileEffect : selectedTiles)
	{
		FLAT_ASSERT(tileEffect.effect > 0.f);
		func(tileEffect.tileIndex, tileEffect.effect);
	}
}

void MapEditorMode::eachBrushTile(std::function<void(map::TileIndex, float)> func) const
{
	for (const map::brush::TileEffect& tileEffect : m_brushTiles)
	{
		FLAT_ASSERT(tileEffect.effect > 0.f);
		func(tileEffect.tileIndex, tileEffect.effect);
	}
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


