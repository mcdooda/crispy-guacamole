#ifndef GAME_STATES_GAMESTATE_H
#define GAME_STATES_GAMESTATE_H

#include <flat.h>
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

	private:
#ifdef FLAT_DEBUG
		void setGamePause(Game& game, bool pause);
		void toggleGamePause(Game& game);
		inline bool isGamePaused() const { return m_gamePaused; }
#endif

	private:
#ifdef FLAT_DEBUG
		bool m_gamePaused;
		bool m_pauseNextFrame;
#endif
};

} // states
} // game

#endif // GAME_STATES_GAMESTATE_H


