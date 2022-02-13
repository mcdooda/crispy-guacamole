#include <unordered_set>
#include "states/mapeditor/tilemapeditormode.h"
#include "states/mapeditorstate.h"
#include "map/tiletemplate.h"
#include "map/brush/spherebrush.h"
#include "game.h"

namespace game
{
namespace states
{
namespace editor
{

TileMapEditorMode::TileMapEditorMode(Game& game) : Super(game)
{

}

void TileMapEditorMode::enter(MapEditorState& mapEditorState)
{
	std::unique_ptr<map::brush::Brush> brush = std::make_unique<map::brush::SphereBrush>();
	brush->setRadius(3.f);
	m_brush = std::move(brush);

	Super::enter(mapEditorState);
}

void TileMapEditorMode::updateBrushTiles(MapEditorState& mapEditorState)
{
	const auto& keyboard = mapEditorState.m_gameInputContext->getKeyboardInputContext();
	const auto& mouse = mapEditorState.m_gameInputContext->getMouseInputContext();

	map::brush::Brush* brush = m_brush.get();
	FLAT_ASSERT(brush != nullptr);
	if (mouse.wheelJustMoved() && keyboard.isPressed(K(SPACE)))
	{
		const flat::Vector2& wheelMove = mouse.getWheelMove();
		float radius = brush->getRadius();
		radius += wheelMove.y;
		radius = std::max(std::min(radius, 30.f), 1.f);
		brush->setRadius(radius);
	}

	FLAT_CHECK_FLOAT(m_brushPosition);

	m_brushTiles.clear();
	brush->getTiles(mapEditorState.getMap(), m_brushPosition, m_brushTiles);

	m_brushTileSlots.clear();
	brush->getTileSlots(mapEditorState.getMap(), m_brushPosition, m_brushTileSlots);

	if (mouse.isPressed(M(RIGHT)))
	{
		if (mouse.isJustPressed(M(RIGHT)) && !keyboard.isPressed(K(LSHIFT)))
		{
			m_selectedTiles.clear();
		}

		eachBrushTile([this](map::TileIndex tileIndex, float effect)
		{
			map::brush::TilesContainer::iterator it = std::find_if(
				m_selectedTiles.begin(),
				m_selectedTiles.end(),
				[tileIndex](const map::brush::TileEffect& tileEffect)
				{
					return tileEffect.tileIndex == tileIndex;
				}
			);
			if (it == m_selectedTiles.end())
			{
				m_selectedTiles.emplace_back(tileIndex, effect);
			}
			else if (effect > it->effect)
			{
				it->effect = effect;
			}
		});
	}
}

void TileMapEditorMode::applyBrushPrimaryEffect(MapEditorState& mapEditorState, bool justPressed)
{
	FLAT_ASSERT(m_tileTemplate != nullptr);

	map::Map& map = mapEditorState.getMap();

	std::unordered_set<map::TileIndex> tilesToUpdate;
	eachBrushTile([this, &map, &tilesToUpdate](map::TileIndex tileIndex, float effect)
	{
		float random = m_game.random->nextFloat(0.f, 1.f);
		if (random <= effect)
		{
			map.setTileTemplate(tileIndex, m_tileTemplate);
		}
	});
}

void TileMapEditorMode::applyBrushSecondaryEffect(MapEditorState& mapEditorState, bool justPressed)
{
	// nothing to do
}

void TileMapEditorMode::handleShortcuts(MapEditorState& mapEditorState)
{
	const float dt = mapEditorState.getGameClock().getDT();
	map::Map& map = mapEditorState.getMap();

	const auto& keyboard = mapEditorState.m_gameInputContext->getKeyboardInputContext();

	const bool altPressed = keyboard.isPressed(K(LALT));
	const bool upPressed   = altPressed ? keyboard.isJustPressed(K(W)) : keyboard.isPressed(K(W));
	const bool downPressed = altPressed ? keyboard.isJustPressed(K(S)) : keyboard.isPressed(K(S));
	if (upPressed || downPressed)
	{
		float displacement = upPressed ? 1.f : -1.f;
		if (!altPressed)
		{
			displacement *= 10.f * dt;
		}
		else
		{
			displacement *= 0.2f;
		}
		eachSelectedTile([this, &map, displacement](map::TileIndex tileIndex, float effect)
		{
			map.setTileZ(tileIndex, map.getTileZ(tileIndex) + displacement * effect);
		});
	}

	if (keyboard.isPressed(K(F)))
	{
		float mean = 0.f;
		float n = 0.f;
		eachSelectedTile([&mean, &n, &map](map::TileIndex tileIndex, float effect)
		{
			mean += map.getTileZ(tileIndex) * effect;
			n += effect;
		});
		mean /= n;
		eachSelectedTile([this, &map, mean, dt](map::TileIndex tileIndex, float effect)
		{
			float z = map.getTileZ(tileIndex);
			map.setTileZ(tileIndex, z + (mean - z) * effect * dt * 5.f);
		});
	}

	if (keyboard.isJustPressed(K(R)))
	{
		map.eachTile([&map](map::TileIndex tileIndex)
		{
			map.setTileZ(tileIndex, 0.f);
		});
	}

	if (keyboard.isJustPressed(K(M)))
	{
		map.eachTile([&map](map::TileIndex tileIndex)
		{
			map.setTileZ(tileIndex, std::round(map.getTileZ(tileIndex)));
		});
	}

	if (keyboard.isJustPressed(K(DELETE)))
	{
		std::vector<map::TileIndex> tilesToDelete;
		eachSelectedTile([this, &map, &tilesToDelete](map::TileIndex tileIndex, float effect)
		{
			map.eachTileEntity(
				tileIndex,
				[](entity::Entity* entity)
				{
					entity->markForDelete();
				}
			);
			tilesToDelete.push_back(tileIndex);
		});

		// sort indices in reverse order to avoid invalidating a further index with deleteTile()
		std::sort(
			tilesToDelete.begin(),
			tilesToDelete.end(),
			[](map::TileIndex a, map::TileIndex b)
			{
				return a > b;
			}
		);
		for (map::TileIndex tileIndex : tilesToDelete)
		{
			// remove from brush tiles
			for (map::brush::TilesContainer::iterator it = m_brushTiles.begin(); it != m_brushTiles.end(); )
			{
				if (it->tileIndex == tileIndex)
				{
					it = m_brushTiles.erase(it);
				}
				else
				{
					++it;
				}
			}

			// remove from selected tiles
			for (map::brush::TilesContainer::iterator it = m_selectedTiles.begin(); it != m_selectedTiles.end(); )
			{
				if (it->tileIndex == tileIndex)
				{
					it = m_selectedTiles.erase(it);
				}
				else
				{
					++it;
				}
			}

			map.deleteTile(tileIndex);
		}
	}

	if (keyboard.isPressed(K(INSERT)))
	{
		eachBrushTileSlot([this, &map](const flat::Vector2i& position, float effect)
		{
			const map::TileIndex tileIndex = map.getTileIndex(position);
			if (!map::isValidTile(tileIndex))
			{
				map.createTile(position, 0.f, m_tileTemplate, 0);
			}
		});
	}
}

void TileMapEditorMode::preDraw(Game& game)
{
	map::Map& map = game.getStateMachine().getState()->as<MapEditorState>().getMap();
	m_temporaryTiles.reserve(m_brushTileSlots.size());
	flat::render::SpriteSynchronizer& synchronizer = map.getTileSpriteSynchronizer(m_tileTemplate, 0);
	for (const map::brush::TileSlotEffect& tileSlotEffect : m_brushTileSlots)
	{
		if (!map::isValidTile(map.getTileIndex(tileSlotEffect.position)))
		{
			map::Tile& tile = m_temporaryTiles.emplace_back();
			const flat::Vector3 position(tileSlotEffect.position.x, tileSlotEffect.position.y, 0.f);
			const flat::Vector2 position2d(map.getTransform() * position);
			tile.synchronizeSpriteTo(map, synchronizer);
			tile.setSpritePosition(position2d);
			tile.updateWorldSpaceAABB(position);
			tile.getSprite()->setColor(flat::video::Color(1.f, 1.f, 1.f, tileSlotEffect.effect));
			map.getDisplayManager().addTemporaryObject(&tile);
		}
	}
}

void TileMapEditorMode::postDraw(Game& game)
{
	m_temporaryTiles.clear();
}

} // editor
} // states
} // game


