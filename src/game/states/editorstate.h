#ifndef GAME_STATES_EDITORSTATE_H
#define GAME_STATES_EDITORSTATE_H

#include "basemapstate.h"
#include "../map/brush/brush.h"

namespace game
{
namespace states
{

class EditorState : public BaseMapState
{
	typedef BaseMapState Super;
	public:
		void enter(flat::state::Agent* agent) override;
		void execute(flat::state::Agent* agent) override;
		
	private:
		std::unique_ptr<map::brush::Brush> m_brush;
		std::vector<map::Tile*> m_brushTiles;
};

} // states
} // game

#endif // GAME_STATES_EDITORSTATE_H


