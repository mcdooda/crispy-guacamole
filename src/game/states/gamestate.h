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
		void enter(flat::state::Agent* agent) override;
		void execute(flat::state::Agent* agent) override;
		
	private:
		entity::Entity* m_sheep;
};

} // states
} // game

#endif // GAME_STATES_GAMESTATE_H


