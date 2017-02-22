#ifndef GAME_STATES_EDITOR_ZONEMODE_H
#define GAME_STATES_EDITOR_ZONEMODE_H

#include "editormode.h"
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

class ZoneEditorMode : public EditorMode
{
	typedef EditorMode Super;
	public:
		ZoneEditorMode(Game& game);
		~ZoneEditorMode() override;

		void clearBrush() const override;
		void displayBrush() const override;
		void updateBrushTiles() override;
		void handleShortcuts() override;

		void setCurrentZone(const std::shared_ptr<map::Zone>& currentZone);
		const std::shared_ptr<map::Zone>& addZone(const std::string& zoneName);
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

#endif // GAME_STATES_EDITOR_ZONEMODE_H


