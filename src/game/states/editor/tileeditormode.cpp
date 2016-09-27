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

TileEditorMode::TileEditorMode(Game* game, EditorState* editorState) : Super(game, editorState)
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
	const flat::input::Keyboard* keyboard = m_game->input->keyboard;
	const flat::input::Mouse* mouse = m_game->input->mouse;

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

	m_previousBrushTiles = std::move(m_brushTiles);
	brush->getTiles(m_editorState->getMap(), m_brushPosition, m_brushTiles);
}

void TileEditorMode::displayBrush() const
{
	map::brush::Brush* brush = m_brush.get();
	FLAT_ASSERT(brush != nullptr);

	for (map::Tile* tile : m_previousBrushTiles)
	{
		tile->setColor(flat::video::Color::WHITE);
	}

	eachBrushTile([](map::Tile* tile, float effect)
	{
		flat::video::Color color(1.f, 1.f - effect, 1.f - effect, 1.f);
		tile->setColor(color);
	});
}

void TileEditorMode::applyBrushPrimaryEffect(bool justPressed) const
{
	eachBrushTileIfExists([this](map::Tile* tile, float effect)
	{
		float random = m_game->random->nextFloat(0.f, 1.f);
		if (random <= effect)
		{
			std::shared_ptr<const flat::video::Texture> texture = m_tileTemplate->getRandomTexture(m_game);
			tile->setTexture(texture);
		}
	});
}

void TileEditorMode::applyBrushSecondaryEffect(bool justPressed) const
{
	bool exists = m_game->input->keyboard->isPressed(K(SPACE));
	if (exists)
	{
		eachBrushTile([this](map::Tile* tile, float effect)
		{
			float random = m_game->random->nextFloat(0.f, 1.f);
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
	else
	{
		eachBrushTile([this](map::Tile* tile, float effect)
		{
			float random = m_game->random->nextFloat(0.f, 1.f);
			if (random <= effect)
			{
				tile->setExists(false);
			}
		});
	}
}

void TileEditorMode::handleShortcuts() const
{
	map::brush::Brush* brush = m_brush.get();
	FLAT_ASSERT(brush != nullptr);

	const float frameTime = m_game->time->getFrameTime();
	map::Map& map = m_editorState->getMap();

	const flat::input::Keyboard* keyboard = m_game->input->keyboard;
	bool upPressed = keyboard->isPressed(K(W));
	bool downPressed = keyboard->isPressed(K(S));
	if (upPressed || downPressed)
	{
		float displacement = (upPressed ? frameTime : -frameTime) * 10.f;
		eachBrushTileIfExists([this, &map, displacement](map::Tile* tile, float effect)
		{
			tile->setZ(map, tile->getZ() + displacement * effect);
		});
	}

	if (keyboard->isPressed(K(F)))
	{
		float mean = 0.f;
		float n = 0.f;
		eachBrushTileIfExists([&mean, &n](map::Tile* tile, float effect)
		{
			mean += tile->getZ() * effect;
			n += effect;
		});
		mean /= n;
		eachBrushTileIfExists([this, &map, mean, frameTime](map::Tile* tile, float effect)
		{
			float z = tile->getZ();
			tile->setZ(map, z + (mean - z) * effect * frameTime * 5.f);
		});
	}

	if (keyboard->isPressed(K(R)))
	{
		map.eachTile([&map](map::Tile* tile)
		{
			if (tile->exists())
			{
				tile->setZ(map, 0.f);
			}
		});
	}
}

} // editor
} // states
} // game


