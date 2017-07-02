#ifndef GAME_STATES_EDITOR_EDITORMODE_H
#define GAME_STATES_EDITOR_EDITORMODE_H

#include <vector>
#include <functional>
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
class MapEditorState;
namespace editor
{

class EditorMode : public flat::util::Convertible<EditorMode>
{
public:
	EditorMode(Game& game);
	virtual ~EditorMode();

	inline const flat::Vector2& getBrushPosition() const { return m_brushPosition; }

	void updateBrushPosition();
	virtual void updateBrushTiles() {}
	virtual void clearBrush() const;
	virtual void displayBrush() const;
	virtual void applyBrushPrimaryEffect(bool justPressed) {}
	virtual void applyBrushSecondaryEffect(bool justPressed) {}
	virtual void handleShortcuts() {}
	virtual bool canSelectEntities() const { return false; }

	virtual void draw() const {}

protected:
	void eachSelectedTile(std::function<void(map::Tile*, float)> func) const;
	void eachSelectedTileIfExists(std::function<void(map::Tile*, float)> func) const;
	void eachBrushTile(std::function<void(map::Tile*, float)> func) const;
	void eachBrushTileIfExists(std::function<void(map::Tile*, float)> func) const;
	void clearSelectedTiles();
	states::MapEditorState& getEditorState() const;
	map::Map& getMap() const;
	const flat::time::Clock& getClock() const;

protected:
	Game& m_game;

	map::brush::TilesContainer m_selectedTiles;
	map::brush::TilesContainer m_brushTiles;
	flat::Vector2 m_brushPosition;
	bool m_brushOnTile;
	std::unique_ptr<map::brush::Brush> m_brush;
};

} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_EDITORMODE_H


