#ifndef GAME_STATES_EDITOR_TILEEDITORMODE_H
#define GAME_STATES_EDITOR_TILEEDITORMODE_H

#include "editormode.h"

namespace game
{
namespace states
{
namespace editor
{

class TileEditorMode : public EditorMode
{
	typedef EditorMode Super;
	public:
		TileEditorMode(Game* game, EditorState* editorState);
		~TileEditorMode() override;

		void updateBrushTiles() override;
		void displayBrush() const override;
		void applyBrush() const override;
		void handleShortcuts() const override;
};

} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_TILEEDITORMODE_H


