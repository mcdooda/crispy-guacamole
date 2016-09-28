#include "propeditormode.h"
#include "../../game.h"
#include "../editorstate.h"
#include "../../map/prop.h"
#include "../../map/proptemplate.h"
#include "../../map/brush/spherebrush.h"

namespace game
{
namespace states
{
namespace editor
{

PropEditorMode::PropEditorMode(Game* game, EditorState* editorState) : Super(game, editorState)
{
	map::brush::Brush* brush = new map::brush::SphereBrush();
	brush->setRadius(3.f);
	m_brush.reset(brush);
}

PropEditorMode::~PropEditorMode()
{
	
}

void PropEditorMode::updateBrushTiles()
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

void PropEditorMode::displayBrush() const
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

void PropEditorMode::applyBrushPrimaryEffect(bool justPressed) const
{
	eachBrushTileIfExists([this](map::Tile* tile, float effect)
	{
		if (tile->getEntities().empty())
		{
			float random = m_game->random->nextFloat(0.f, 1.f);
			if (random <= effect)
			{
				std::shared_ptr<const flat::video::Texture> texture = m_propTemplate->getRandomTexture(m_game);
				tile->setPropTexture(texture);
			}
		}
	});
}

void PropEditorMode::applyBrushSecondaryEffect(bool justPressed) const
{
	eachBrushTileIfExists([this](map::Tile* tile, float effect)
	{
		float random = m_game->random->nextFloat(0.f, 1.f);
		if (random <= effect)
		{
			if (tile->getProp() != nullptr)
			{
				tile->removeProp();
			}
		}
	});
}

} // editor
} // states
} // game


