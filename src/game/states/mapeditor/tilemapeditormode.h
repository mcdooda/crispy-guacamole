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

		void enter(MapEditorState& mapEditorState) override;

		void updateBrushTiles(MapEditorState& mapEditorState) override;
		void applyBrushPrimaryEffect(MapEditorState& mapEditorState, bool justPressed) override;
		void applyBrushSecondaryEffect(MapEditorState& mapEditorState, bool justPressed) override;
		void handleShortcuts(MapEditorState& mapEditorState) override;

		inline void setTileTemplate(const std::shared_ptr<const map::TileTemplate>& tileTemplate) { m_tileTemplate = tileTemplate; }

	private:
		std::shared_ptr<const map::TileTemplate> m_tileTemplate;
};

} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_TILEMAPEDITORMODE_H


