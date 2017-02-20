#include "tileeditormode.h"
#include "../../game.h"
#include "../editorstate.h"
#include "../../map/tile.h"
#include "../../map/tiletemplate.h"
#include "../../map/brush/spherebrush.h"

namespace game
{
namespace states
{
namespace editor
{

TileEditorMode::TileEditorMode(Game& game) : Super(game)
{
	map::brush::Brush* brush = new map::brush::SphereBrush();
	brush->setRadius(3.f);
	m_brush.reset(brush);
}

TileEditorMode::~TileEditorMode()
{

}

void TileEditorMode::updateBrushTiles()
{
	const flat::input::Keyboard* keyboard = m_game.input->keyboard;
	const flat::input::Mouse* mouse = m_game.input->mouse;

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
	brush->getTiles(getMap(), m_brushPosition, m_brushTiles);

	if (mouse->isPressed(M(RIGHT)))
	{
		if (mouse->isJustPressed(M(RIGHT)) && !keyboard->isPressed(K(LSHIFT)))
		{
			m_selectedTiles.clear();
		}

		eachBrushTile([this](map::Tile* tile, float effect)
		{
			std::map<map::Tile*, float>::iterator it = m_selectedTiles.find(tile);
			if (it == m_selectedTiles.end() || effect > it->second)
			{
				m_selectedTiles[tile] = effect;
			}
		});
	}
}

void TileEditorMode::applyBrushPrimaryEffect(bool justPressed)
{
	eachBrushTileIfExists([this](map::Tile* tile, float effect)
	{
		float random = m_game.random->nextFloat(0.f, 1.f);
		if (random <= effect)
		{
			std::shared_ptr<const flat::video::Texture> texture = m_tileTemplate->getRandomTexture(m_game);
			tile->setTexture(texture);
		}
	});
	clearSelectedTiles();
}

void TileEditorMode::applyBrushSecondaryEffect(bool justPressed)
{
	// nothing to do
}

void TileEditorMode::handleShortcuts()
{
	const float frameTime = m_game.time->getFrameTime();
	map::Map& map = getMap();

	const flat::input::Keyboard* keyboard = m_game.input->keyboard;

	const bool altPressed = keyboard->isPressed(K(LALT));
	const bool upPressed   = altPressed ? keyboard->isJustPressed(K(W)) : keyboard->isPressed(K(W));
	const bool downPressed = altPressed ? keyboard->isJustPressed(K(S)) : keyboard->isPressed(K(S));
	if (upPressed || downPressed)
	{
		float displacement = upPressed ? 1.f : -1.f;
		if (!altPressed)
		{
			displacement *= 10.f * frameTime;
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
		eachSelectedTileIfExists([this, &map, mean, frameTime](map::Tile* tile, float effect)
		{
			float z = tile->getZ();
			tile->setZ(map, z + (mean - z) * effect * frameTime * 5.f);
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
		eachSelectedTileIfExists([](map::Tile* tile, float effect)
		{
			tile->setExists(false);
			for (entity::Entity* entity : tile->getEntities())
			{
				entity->markForDelete();
			}
		});
	}

	if (keyboard->isPressed(K(SPACE)))
	{
		eachBrushTile([this](map::Tile* tile, float effect)
		{
			float random = m_game.random->nextFloat(0.f, 1.f);
			if (random <= effect)
			{
				tile->setExists(true);
				if (!tile->hasSprite())
				{
					std::shared_ptr<const flat::video::Texture> texture = m_tileTemplate->getRandomTexture(m_game);
					tile->setTexture(texture);
				}
			}
		});
	}
}

} // editor
} // states
} // game


