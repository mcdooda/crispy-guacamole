#include "propmapeditormode.h"
#include "../mapeditorstate.h"
#include "../../game.h"
#include "../../map/prop.h"
#include "../../map/proptemplate.h"
#include "../../map/brush/spherebrush.h"

namespace game
{
namespace states
{
namespace editor
{

PropMapEditorMode::PropMapEditorMode(Game& game) : Super(game)
{

}

void PropMapEditorMode::enter(MapEditorState& mapEditorState)
{
	std::unique_ptr<map::brush::Brush> brush = std::make_unique<map::brush::SphereBrush>();
	brush->setRadius(3.f);
	m_brush = std::move(brush);

	Super::enter(mapEditorState);
}

void PropMapEditorMode::updateBrushTiles(MapEditorState& mapEditorState)
{
	const auto& keyboard = mapEditorState.m_gameInputContext->getKeyboardInputContext();
	const auto& mouse = mapEditorState.m_gameInputContext->getMouseInputContext();

	map::brush::Brush* brush = m_brush.get();
	FLAT_ASSERT(brush != nullptr);
	if (mouse.wheelJustMoved() && !keyboard.isPressed(K(LCTRL)))
	{
		const flat::Vector2& wheelMove = mouse.getWheelMove();
		float radius = brush->getRadius();
		radius += wheelMove.y;
		radius = std::max(std::min(radius, 30.f), 1.f);
		brush->setRadius(radius);
	}


	m_brushTiles.clear();
	brush->getTiles(mapEditorState.getMap(), m_brushPosition, m_brushTiles);
}

void PropMapEditorMode::applyBrushPrimaryEffect(MapEditorState& mapEditorState, bool justPressed)
{
	map::Map& map = mapEditorState.getMap();
	eachBrushTile([this, &map](map::TileIndex tileIndex, float effect)
	{
		if (map.getTileEntityCount(tileIndex) == 0)
		{
			float random = m_game.random->nextFloat(0.f, 1.f);
			if (random <= effect)
			{
				std::shared_ptr<const flat::video::Texture> texture = m_propTemplate->getRandomTexture(m_game);
				map.setTilePropTexture(tileIndex, texture);
			}
		}
	});
}

void PropMapEditorMode::applyBrushSecondaryEffect(MapEditorState& mapEditorState, bool justPressed)
{
	map::Map& map = mapEditorState.getMap();
	eachBrushTile([this, &map](map::TileIndex tileIndex, float effect)
	{
		float random = m_game.random->nextFloat(0.f, 1.f);
		if (random <= effect)
		{
			map.removeTileProp(tileIndex);
		}
	});
}

} // editor
} // states
} // game


