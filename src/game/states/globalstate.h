#ifndef GAME_STATES_GLOBALSTATE_H
#define GAME_STATES_GLOBALSTATE_H

#include <flat.h>

namespace game
{
class Game;
namespace states
{

class GlobalState : public flat::state::State
{
	public:
		void enter(flat::state::Agent* agent) override;
		void execute(flat::state::Agent* agent) override;
		void exit(flat::state::Agent* agent) override;
		
	private:
		void resetViews(game::Game* game);
};

} // states
} // game

#endif // GAME_STATES_GLOBALSTATE_H


