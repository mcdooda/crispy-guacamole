#include "tilemapeditormode.h"
#include "../mapeditorstate.h"
#include "../../game.h"
#include "../../map/tile.h"
#include "../../map/prop.h"
#include "../../map/tiletemplate.h"
#include "../../map/brush/spherebrush.h"

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
	const auto& keyboard = m_game.input->keyboard;
	const auto& mouse = m_game.input->mouse;

	map::brush::Brush* brush = m_brush.get();
	FLAT_ASSERT(brush != nullptr);
	if (mouse->wheelJustMoved() && !keyboard->isPressed(K(LCTRL)))
	{
		const flat::Vector2& wheelMove = mouse->getWheelMove();
		float radius = brush->getRadius();
		radius += wheelMove.y;
		radius = std::max(std::min(radius, 30.f), 1.f);
		brush->setRadius(radius);
	}

	m_brushTiles.clear();
	brush->getTiles(mapEditorState.getMap(), m_brushPosition, m_brushTiles);

	if (mouse->isPressed(M(RIGHT)))
	{
		if (mouse->isJustPressed(M(RIGHT)) && !keyboard->isPressed(K(LSHIFT)))
		{
			m_selectedTiles.clear();
		}

		eachBrushTile([this](map::Tile* tile, float effect)
		{
			map::brush::TilesContainer::iterator it = std::find_if(
				m_selectedTiles.begin(),
				m_selectedTiles.end(),
				[tile](const map::brush::TileEffect& tileEffect)
				{
					return tileEffect.tile == tile;
				}
			);
			if (it == m_selectedTiles.end() || effect > it->effect)
			{
				m_selectedTiles.emplace_back(tile, effect);
			}
		});
	}
}

void TileMapEditorMode::applyBrushPrimaryEffect(MapEditorState& mapEditorState, bool justPressed)
{
	FLAT_ASSERT(m_tileTemplate != nullptr);

	map::Map& map = mapEditorState.getMap();
	eachBrushTileIfExists([this, &map](map::Tile* tile, float effect)
	{
		float random = m_game.random->nextFloat(0.f, 1.f);
		if (random <= effect)
		{
			int tileVariantIndex = m_tileTemplate->getRandomTileVariantIndex(m_game);
			flat::render::SpriteSynchronizer& spriteSynchronizer = map.getTileSpriteSynchronizer(m_tileTemplate, tileVariantIndex);
			tile->synchronizeSpriteTo(map, spriteSynchronizer);
		}
	});
	clearSelectedTiles();
}

void TileMapEditorMode::applyBrushSecondaryEffect(MapEditorState& mapEditorState, bool justPressed)
{
	// nothing to do
}

void TileMapEditorMode::handleShortcuts(MapEditorState& mapEditorState)
{
	const float dt = mapEditorState.getClock().getDT();
	map::Map& map = mapEditorState.getMap();

	const auto& keyboard = m_game.input->keyboard;

	const bool altPressed = keyboard->isPressed(K(LALT));
	const bool upPressed   = altPressed ? keyboard->isJustPressed(K(W)) : keyboard->isPressed(K(W));
	const bool downPressed = altPressed ? keyboard->isJustPressed(K(S)) : keyboard->isPressed(K(S));
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
		eachSelectedTileIfExists([this, &map, displacement](map::Tile* tile, float effect)
		{
			tile->setZ(map, tile->getZ() + displacement * effect);
		});
	}

	if (keyboard->isPressed(K(F)))
	{
		float mean = 0.f;
		float n = 0.f;
		eachSelectedTileIfExists([&mean, &n](map::Tile* tile, float effect)
		{
			mean += tile->getZ() * effect;
			n += effect;
		});
		mean /= n;
		eachSelectedTileIfExists([this, &map, mean, dt](map::Tile* tile, float effect)
		{
			float z = tile->getZ();
			tile->setZ(map, z + (mean - z) * effect * dt * 5.f);
		});
	}

	if (keyboard->isJustPressed(K(R)))
	{
		map.eachTileIfExists([&map](map::Tile* tile)
		{
			tile->setZ(map, 0.f);
		});
	}

	if (keyboard->isJustPressed(K(DELETE)))
	{
		eachSelectedTileIfExists([this, &map](map::Tile* tile, float effect)
		{
			tile->setExists(map, false);
			for (entity::Entity* entity : tile->getEntities())
			{
				entity->markForDelete();
			}
		});
	}

	if (keyboard->isPressed(K(SPACE)))
	{
		eachBrushTile([this, &map](map::Tile* tile, float effect)
		{
			if (!tile->exists())
			{
				float random = m_game.random->nextFloat(0.f, 1.f);
				if (random <= effect)
				{
					if (!tile->hasSprite())
					{
						int tileVariantIndex = m_tileTemplate->getRandomTileVariantIndex(m_game);
						flat::render::SpriteSynchronizer& spriteSynchronizer = map.getTileSpriteSynchronizer(m_tileTemplate, tileVariantIndex);
						tile->synchronizeSpriteTo(map, spriteSynchronizer);
					}
					tile->setExists(map, true);
				}
			}
		});
	}
}

} // editor
} // states
} // game


