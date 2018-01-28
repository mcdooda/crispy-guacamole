#ifndef GAME_STATES_EDITOR_ZONEMAPEDITORMODE_H
#define GAME_STATES_EDITOR_ZONEMAPEDITORMODE_H

#include "mapeditormode.h"
#include "../../map/zone.h"

namespace game
{
namespace map
{
class Zone;
}
namespace states
{
namespace editor
{

class ZoneMapEditorMode : public MapEditorMode
{
	using Super = MapEditorMode;
	public:
		ZoneMapEditorMode(Game& game);
		~ZoneMapEditorMode() override;

		void clearBrush(MapEditorState& mapEditorState) const override;
		void displayBrush(MapEditorState& mapEditorState) const override;
		void updateBrushTiles(MapEditorState& mapEditorState) override;
		void handleShortcuts(MapEditorState& mapEditorState) override;

		void setCurrentZone(MapEditorState& mapEditorState, const std::shared_ptr<map::Zone>& currentZone);
		const std::shared_ptr<map::Zone>& addZone(MapEditorState& mapEditorState, const std::string& zoneName);
		bool removeZone(MapEditorState& mapEditorState, const std::string& zoneName);
		inline const map::Zone* getCurrentZone() const { return m_currentZone.lock().get(); }

	private:
		std::weak_ptr<map::Zone> m_currentZone;
		map::Zone::Rectangle m_currentRectangle;
		flat::Vector2 m_mouseDownMapPosition;
		const map::Zone::Rectangle* m_selectedRectangle;
		bool m_drawingRectangle : 1;
};

} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_ZONEMAPEDITORMODE_H


