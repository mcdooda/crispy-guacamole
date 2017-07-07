#ifndef GAME_STATES_EDITOR_TILEMAPEDITORMODE_H
#define GAME_STATES_EDITOR_TILEMAPEDITORMODE_H

#include "mapeditormode.h"

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

class TileMapEditorMode : public MapEditorMode
{
	using Super = MapEditorMode;
	public:
		TileMapEditorMode(Game& game);
		~TileMapEditorMode() override;

		void updateBrushTiles() override;
		void applyBrushPrimaryEffect(bool justPressed) override;
		void applyBrushSecondaryEffect(bool justPressed) override;
		void handleShortcuts() override;

		inline void setTileTemplate(const std::shared_ptr<const map::TileTemplate>& tileTemplate) { m_tileTemplate = tileTemplate; }

	private:
		std::shared_ptr<const map::TileTemplate> m_tileTemplate;
};

} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_TILEMAPEDITORMODE_H


