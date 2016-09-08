#include <algorithm>
#include "editorstate.h"
#include "../game.h"
#include "../map/tile.h"
#include "../map/brush/spherebrush.h"

namespace game
{
namespace states
{

void EditorState::enter(flat::state::Agent* agent)
{
	Super::enter(agent);
	
	setCameraZoom(0.5f);
	
	flat::lua::doFile(m_luaState, "data/editor/scripts/ui.lua");
	
	map::brush::Brush* brush = new map::brush::SphereBrush();
	brush->setRadius(3.f);
	m_brush.reset(brush);
}

void EditorState::execute(flat::state::Agent* agent)
{
	Game* game = agent->to<Game>();

	updateBrush(game);
	displayBrush(game);
	applyBrush(game);
	
	Super::execute(agent);
}

void EditorState::updateBrush(Game* game)
{
	const flat::input::Keyboard* keyboard = game->input->keyboard;
	const flat::input::Mouse* mouse = game->input->mouse;

	m_brushPosition = getCursorMapPosition(game);
	if (keyboard->isPressed(K(LSHIFT)))
	{
		m_brushPosition.x = std::round(m_brushPosition.x);
		m_brushPosition.y = std::round(m_brushPosition.y);
	}

	map::brush::Brush* brush = m_brush.get();
	FLAT_ASSERT(brush != nullptr);
	if (mouse->wheelJustMoved() && !keyboard->isPressed(K(LCTRL)))
	{
		const flat::Vector2& wheelMove = game->input->mouse->getWheelMove();
		float radius = brush->getRadius();
		radius += wheelMove.y;
		radius = std::max(std::min(radius, 30.f), 1.f);
		brush->setRadius(radius);
	}
}

void EditorState::displayBrush(Game* game)
{
	map::brush::Brush* brush = m_brush.get();
	FLAT_ASSERT(brush != nullptr);

	for (map::Tile* tile : m_brushTiles)
	{
		tile->setColor(flat::video::Color::WHITE);
	}

	brush->getTiles(m_map, m_brushPosition, m_brushTiles);

	eachBrushTile([](map::Tile* tile, float effect)
	{
		flat::video::Color color(1.f, 1.f - effect, 1.f - effect, 1.f);
		tile->setColor(color);
	});
}

void EditorState::applyBrush(Game* game)
{
	map::brush::Brush* brush = m_brush.get();
	FLAT_ASSERT(brush != nullptr);

	const float frameTime = game->time->getFrameTime();

	const flat::input::Keyboard* keyboard = game->input->keyboard;
	bool upPressed = keyboard->isPressed(K(PAGEUP));
	bool downPressed = keyboard->isPressed(K(PAGEDOWN));
	if (upPressed || downPressed)
	{
		float displacement = (upPressed ? frameTime : -frameTime) * 10.f;
		applyDisplacement(displacement);
	}

	if (keyboard->isPressed(K(F)))
	{
		applyMean(frameTime);
	}

	if (keyboard->isPressed(K(W)))
	{
		applyZero();
	}
}

void EditorState::applyDisplacement(float displacement)
{
	eachBrushTile([this, displacement](map::Tile* tile, float effect)
	{
		tile->setZ(m_map, tile->getZ() + displacement * effect);
	});
}

void EditorState::applyMean(float frameTime)
{
	float mean = 0.f;
	float n = 0.f;
	eachBrushTile([&mean, &n](map::Tile* tile, float effect)
	{
		mean += tile->getZ() * effect;
		n += effect;
	});
	mean /= n;
	eachBrushTile([this, mean, frameTime](map::Tile* tile, float effect)
	{
		float z = tile->getZ();
		tile->setZ(m_map, z + (mean - z) * effect * frameTime * 5.f);
	});
}

void EditorState::applyZero()
{
	m_map.eachTile([this](map::Tile* tile)
	{
		if (tile->exists())
		{
			tile->setZ(m_map, 0.f);
		}
	});
}

void EditorState::eachBrushTile(std::function<void(map::Tile*, float)> func)
{
	map::brush::Brush* brush = m_brush.get();
	for (map::Tile* tile : m_brushTiles)
	{
		if (tile->exists())
		{
			float effect = brush->getTileEffect(m_brushPosition, tile);
			if (effect > 0.f)
			{
				func(tile, effect);
			}
		}
	}
}

} // states
} // game



