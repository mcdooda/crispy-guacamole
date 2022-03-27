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

	m_brushTileSlots.clear();
	brush->getTileSlots(mapEditorState.getMap(), m_brushPosition, m_brushTileSlots);
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

void PropMapEditorMode::preDraw(Game& game)
{
	map::Map& map = game.getStateMachine().getState()->as<MapEditorState>().getMap();
	m_temporaryProps.reserve(m_brushTileSlots.size());
	std::shared_ptr<const flat::video::Texture> propTexture = m_propTemplate->getFirstTexture();
	for (const map::brush::TileSlotEffect& tileSlotEffect : m_brushTileSlots)
	{
		const map::TileIndex tileIndex = map.getTileIndex(tileSlotEffect.position);
		if (map::isValidTile(tileIndex))
		{
			const map::PropIndex propIndex = map.getTilePropIndex(tileIndex);
			if (!map::isValidProp(propIndex) && map.getTileEntityCount(tileIndex) == 0)
			{
				map::Prop& prop = m_temporaryProps.emplace_back();
				const flat::Vector3 position(tileSlotEffect.position.x, tileSlotEffect.position.y, 0.f);
				const flat::Vector3 position2d(map.getTransform() * position);
				prop.setSpritePosition(position2d);
				prop.updateWorldSpaceAABB(position);
				prop.setSpriteTexture(propTexture);
				const flat::Vector2& textureSize = propTexture->getSize();
				flat::Vector2 origin(textureSize.x / 2.f, textureSize.y - textureSize.x / 4.f);
				prop.setSpriteOrigin(origin);
				prop.updateRenderHash();
				prop.getSprite()->setColor(flat::video::Color(1.f, 1.f, 1.f, tileSlotEffect.effect));
				map.getDisplayManager().addTemporaryObject(&prop);
			}
		}
	}
}

void PropMapEditorMode::postDraw(Game& game)
{
	m_temporaryProps.clear();
}

} // editor
} // states
} // game


