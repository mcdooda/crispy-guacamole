#ifndef GAME_STATES_GAMESTATE_H
#define GAME_STATES_GAMESTATE_H

#include "basemapstate.h"

namespace game
{
namespace states
{

class GameState : public BaseMapState
{
	typedef BaseMapState Super;
	public:
		void enter(Game& game) override final;
		void execute(Game& game) override final;
};

} // states
} // game

#endif // GAME_STATES_GAMESTATE_H


