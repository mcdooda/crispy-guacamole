#ifndef GAME_STATES_EDITORSTATE_H
#define GAME_STATES_EDITORSTATE_H

#include "basemapstate.h"

namespace game
{
namespace states
{

class EditorState : public BaseMapState
{
	typedef BaseMapState Super;
	public:
		void enter(flat::state::Agent* agent) override;
};

} // states
} // game

#endif // GAME_STATES_EDITORSTATE_H


