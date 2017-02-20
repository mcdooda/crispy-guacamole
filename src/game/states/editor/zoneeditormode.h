#ifndef GAME_STATES_EDITOR_ZONEMODE_H
#define GAME_STATES_EDITOR_ZONEMODE_H

#include "editormode.h"

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

		void setCurrentZone(const std::shared_ptr<map::Zone>& currentZone);

	private:
		std::weak_ptr<map::Zone> m_currentZone;
};

} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_ZONEMODE_H


