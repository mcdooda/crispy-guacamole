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
	
	flat::Vector2 brushCenter = getCursorMapPosition(game);
	
	map::brush::Brush* brush = m_brush.get();
	FLAT_ASSERT(brush);
	
	for (map::Tile* tile : m_brushTiles)
	{
		tile->setColor(flat::video::Color::WHITE);
	}

	brush->getTiles(m_map, brushCenter, m_brushTiles);
	for (map::Tile* tile : m_brushTiles)
	{
		float effect = brush->getTileEffect(brushCenter, tile);
		flat::video::Color color(1.f, 1.f - effect, 1.f - effect, 1.f);
		tile->setColor(color);
	}
	
	Super::execute(agent);
}

} // states
} // game



