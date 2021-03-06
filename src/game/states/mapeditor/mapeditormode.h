#ifndef GAME_STATES_EDITOR_MAPEDITORMODE_H
#define GAME_STATES_EDITOR_MAPEDITORMODE_H

#include <vector>
#include <functional>
#include "../mapeditorstate.h"
#include "../../map/tile.h"
#include "../../map/brush/brush.h"

namespace game
{
class Game;
namespace map
{
class Map;
}
namespace states
{
namespace editor
{

class MapEditorMode : public flat::state::StateImpl<MapEditorState>
{
public:
	MapEditorMode(Game& game);
	virtual ~MapEditorMode();

	void enter(MapEditorState& mapEditorState) override;
	void execute(MapEditorState& mapEditorState) override;
	void exit(MapEditorState& mapEditorState) override;

	inline const flat::Vector2& getBrushPosition() const { return m_brushPosition; }

	void updateBrushPosition(MapEditorState& mapEditorState);
	virtual void updateBrushTiles(MapEditorState& mapEditorState) {}
	virtual void clearBrush(MapEditorState& mapEditorState) const;
	virtual void displayBrush(MapEditorState& mapEditorState) const;
	virtual void applyBrushPrimaryEffect(MapEditorState& mapEditorState, bool justPressed) {}
	virtual void applyBrushSecondaryEffect(MapEditorState& mapEditorState, bool justPressed) {}
	virtual void handleShortcuts(MapEditorState& mapEditorState) {}
	virtual bool canSelectEntities() const { return false; }

	virtual void preDraw(Game& game) {}
	virtual void postDraw(Game& game) {}

protected:
	void eachSelectedTile(std::function<void(map::TileIndex, float)> func) const;
	void eachBrushTile(std::function<void(map::TileIndex, float)> func) const;
	void eachBrushTileSlot(std::function<void(const flat::Vector2i&, float)> func) const;
	void applyBrush(MapEditorState& mapEditorState);

protected:
	Game& m_game;

	map::brush::TilesContainer m_selectedTiles;
	map::brush::TilesContainer m_brushTiles;
	map::brush::TileSlotsContainer m_brushTileSlots;
	flat::Vector2 m_brushPosition;
	bool m_brushOnTile;
	std::unique_ptr<map::brush::Brush> m_brush;
};

} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_MAPEDITORMODE_H


