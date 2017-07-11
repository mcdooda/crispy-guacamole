#ifndef GAME_STATES_SELECTMAPSTATE_H
#define GAME_STATES_SELECTMAPSTATE_H

#include "basestate.h"

namespace game
{
class Game;
namespace states
{

class SelectMapState : public BaseState
{
	using Super = BaseState;
	public:
		void enter(Game& game) override;
};

} // states
} // game

#endif // GAME_STATES_SELECTMAPSTATE_H


