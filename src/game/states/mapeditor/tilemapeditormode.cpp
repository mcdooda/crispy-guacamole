#include <unordered_set>
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

	m_brushTiles.clear();
	brush->getTiles(mapEditorState.getMap(), m_brushPosition, m_brushTiles);

	if (mouse.isPressed(M(RIGHT)))
	{
		if (mouse.isJustPressed(M(RIGHT)) && !keyboard.isPressed(K(LSHIFT)))
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

FLAT_OPTIMIZE_OFF()
void TileMapEditorMode::applyBrushPrimaryEffect(MapEditorState& mapEditorState, bool justPressed)
{
	FLAT_ASSERT(m_tileTemplate != nullptr);

	map::Map& map = mapEditorState.getMap();

	std::unordered_set<map::Tile*> tilesToUpdate;
	eachBrushTileIfExists([this, &map, &tilesToUpdate](map::Tile* tile, float effect)
	{
		float random = m_game.random->nextFloat(0.f, 1.f);
		if (random <= effect)
		{
			flat::render::SpriteSynchronizer& spriteSynchronizer = map.getTileSpriteSynchronizer(m_tileTemplate, 0);
			tile->synchronizeSpriteTo(map, spriteSynchronizer);

			tilesToUpdate.insert(tile);

			// add adjacent tiles
			auto addToTilesToUpdate = [&tilesToUpdate, &map, tile](int dx, int dy)
			{
				map::Tile* adjacentTile = map.getTileIfExists(tile->getX() + dx, tile->getY() + dy);
				if (adjacentTile != nullptr)
				{
					std::shared_ptr<const map::TileTemplate> tileTemplate = map.getTileTemplate(adjacentTile);
					const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getSelectTile = tileTemplate->getSelectTile();
					if (getSelectTile)
					{
						tilesToUpdate.insert(adjacentTile);
					}
				}
			};
			addToTilesToUpdate(-1, 0);
			addToTilesToUpdate(0, -1);
			addToTilesToUpdate(1, 0);
			addToTilesToUpdate(0, 1);
		}
	});

	for (map::Tile* tile : tilesToUpdate)
	{
		int tileVariantIndex = -1;
		std::shared_ptr<const map::TileTemplate> tileTemplate = map.getTileTemplate(tile);
		const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getSelectTile = tileTemplate->getSelectTile();
		if (getSelectTile)
		{
			getSelectTile.callFunction(
				[&map, tile](lua_State* L)
				{
					auto pushTileTemplateName = [&map, tile, L](int dx, int dy)
					{
						const map::Tile* adjacentTile = map.getTileIfExists(tile->getX() + dx, tile->getY() + dy);
						if (adjacentTile != nullptr)
						{
							lua_pushstring(L, map.getTileTemplate(adjacentTile)->getName().c_str());
						}
						else
						{
							lua_pushnil(L);
						}
					};
					pushTileTemplateName(0, -1);
					pushTileTemplateName(-1, 0);
					pushTileTemplateName(1, 0);
					pushTileTemplateName(0, 1);
				},
				1,
				[this, &tileVariantIndex, &tileTemplate](lua_State* L)
				{
					luaL_checktype(L, -1, LUA_TTABLE);
					size_t numTileVariants = lua_rawlen(L, 1);
					std::vector<int> tileVariantIndices(numTileVariants);
					for (size_t i = 1; i <= numTileVariants; ++i)
					{
						lua_rawgeti(L, 1, i);
						int tileVariantIndex = static_cast<int>(luaL_checkinteger(L, -1));
						tileVariantIndices[i - 1] = tileVariantIndex - 1;
						lua_pop(L, 1);
					}
					tileVariantIndex = tileTemplate->getRandomTileVariantIndex(m_game, tileVariantIndices);
				}
			);
		}
		else
		{
			tileVariantIndex = tileTemplate->getRandomTileVariantIndex(m_game);
		}
		FLAT_ASSERT(tileVariantIndex >= 0);
		flat::render::SpriteSynchronizer& spriteSynchronizer = map.getTileSpriteSynchronizer(tileTemplate, tileVariantIndex);
		tile->synchronizeSpriteTo(map, spriteSynchronizer);
	}
}
FLAT_OPTIMIZE_ON()

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
		eachSelectedTileIfExists([this, &map, displacement](map::Tile* tile, float effect)
		{
			tile->setZ(map, tile->getZ() + displacement * effect);
		});
	}

	if (keyboard.isPressed(K(F)))
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

	if (keyboard.isJustPressed(K(R)))
	{
		map.eachTileIfExists([&map](map::Tile* tile)
		{
			tile->setZ(map, 0.f);
		});
	}

	if (keyboard.isJustPressed(K(DELETE)))
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

	if (keyboard.isPressed(K(SPACE)))
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


