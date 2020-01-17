#ifndef GAME_STATES_EDITOR_TILEMAPEDITORMODE_H
#define GAME_STATES_EDITOR_TILEMAPEDITORMODE_H

#include "mapeditormode.h"
#include "map/tile.h"

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

		void preDraw(Game& game) override;
		void postDraw(Game& game) override;

	private:
		std::shared_ptr<const map::TileTemplate> m_tileTemplate;

		std::vector<map::Tile> m_temporaryTiles;
};

} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_TILEMAPEDITORMODE_H


