#include "propeditormode.h"
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

PropEditorMode::PropEditorMode(Game& game) : Super(game)
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
	brush->getTiles(getMap(), m_brushPosition, m_brushTiles);
}

void PropEditorMode::applyBrushPrimaryEffect(bool justPressed)
{
	map::Map& map = getMap();
	eachBrushTileIfExists([this, &map](map::Tile* tile, float effect)
	{
		if (tile->getEntities().empty())
		{
			float random = m_game.random->nextFloat(0.f, 1.f);
			if (random <= effect)
			{
				std::shared_ptr<const flat::video::Texture> texture = m_propTemplate->getRandomTexture(m_game);
				tile->setPropTexture(map, texture);
			}
		}
	});
}

void PropEditorMode::applyBrushSecondaryEffect(bool justPressed)
{
	map::Map& map = getMap();
	eachBrushTileIfExists([this, &map](map::Tile* tile, float effect)
	{
		float random = m_game.random->nextFloat(0.f, 1.f);
		if (random <= effect)
		{
			if (tile->getProp() != nullptr)
			{
				tile->removeProp(map);
			}
		}
	});
}

} // editor
} // states
} // game


