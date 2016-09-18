#ifndef GAME_STATES_EDITORSTATE_H
#define GAME_STATES_EDITORSTATE_H

#include "basemapstate.h"
#include "editor/editormode.h"
#include "../map/brush/brush.h"

namespace game
{
namespace map
{
class Tile;
}
namespace states
{

class EditorState : public BaseMapState
{
	typedef BaseMapState Super;
	public:
		void enter(flat::state::Agent* agent) override;
		void execute(flat::state::Agent* agent) override;
		
	private:
		void saveOnCtrlS(Game* game);
		void updateBrush(Game* game);
		void displayBrush(Game* game);
		void applyBrush(Game* game);

	private:
		std::unique_ptr<editor::EditorMode> m_editorMode;
};

} // states
} // game

#endif // GAME_STATES_EDITORSTATE_H


