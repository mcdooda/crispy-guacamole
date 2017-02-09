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
class EditorState;
namespace editor
{

class EditorMode : public flat::util::Convertible<EditorMode>
{
public:
	EditorMode(Game& game);
	virtual ~EditorMode();

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
	void eachBrushTile(std::function<void(map::Tile*, float)> func) const;
	states::EditorState& getEditorState() const;
	map::Map& getMap() const;

protected:
	Game& m_game;

	std::map<map::Tile*, float> m_selectedTiles;
	std::map<map::Tile*, float> m_brushTiles;
	flat::Vector2 m_brushPosition;
	std::unique_ptr<map::brush::Brush> m_brush;
	bool m_keepSelection;
};

} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_EDITORMODE_H


