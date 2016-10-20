#ifndef GAME_STATES_EDITOR_TILEEDITORMODE_H
#define GAME_STATES_EDITOR_TILEEDITORMODE_H

#include "editormode.h"

namespace game
{
namespace map
{
class TileTemplate;
}
namespace states
{
namespace editor
{

class TileEditorMode : public EditorMode
{
	typedef EditorMode Super;
	public:
		TileEditorMode(Game& game);
		~TileEditorMode() override;

		void updateBrushTiles() override;
		void displayBrush() const override;
		void applyBrushPrimaryEffect(bool justPressed) const override;
		void applyBrushSecondaryEffect(bool justPressed) const override;
		void handleShortcuts() const override;

		inline void setTileTemplate(const std::shared_ptr<const map::TileTemplate>& tileTemplate) { m_tileTemplate = tileTemplate; }

	private:
		std::shared_ptr<const map::TileTemplate> m_tileTemplate;
};

} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_TILEEDITORMODE_H


