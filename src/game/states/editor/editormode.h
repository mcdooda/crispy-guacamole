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
	virtual void displayBrush() const {}
	virtual void applyBrushPrimaryEffect(bool justPressed) const {}
	virtual void applyBrushSecondaryEffect(bool justPressed) const {}
	virtual void handleShortcuts() const {}

	virtual void draw() const {}

protected:
	void eachBrushTileIfExists(std::function<void(map::Tile*, float)> func) const;
	void eachBrushTile(std::function<void(map::Tile*, float)> func) const;
	states::EditorState& getEditorState() const;
	map::Map& getMap() const;

protected:
	Game& m_game;

	std::vector<map::Tile*> m_previousBrushTiles;
	std::vector<map::Tile*> m_brushTiles;
	flat::Vector2 m_brushPosition;
	std::unique_ptr<map::brush::Brush> m_brush;
};

} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_EDITORMODE_H


