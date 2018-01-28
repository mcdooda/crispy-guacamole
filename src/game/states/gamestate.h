#ifndef GAME_STATES_GAMESTATE_H
#define GAME_STATES_GAMESTATE_H

#include <flat.h>
#include "basemapstate.h"
#include "../map/gamemap.h"

namespace game
{
namespace states
{

class GameState : public BaseMapStateImpl<map::GameMap>
{
	using Super = BaseMapStateImpl<map::GameMap>;
	public:
		void enter(Game& game) override final;
		void execute(Game& game) override final;
		void exit(Game& game) override final;

#ifdef FLAT_DEBUG
		void setGamePause(Game& game, bool pause, bool pauseNextFrame);
		inline bool isGamePaused() const { return m_gamePaused; }
#endif

	private:
		void moveToFormation(Game& game);

	private:
		flat::lua::Thread m_levelThread;
#ifdef FLAT_DEBUG
		bool m_gamePaused;
		bool m_pauseNextFrame;
#endif
};

} // states
} // game

#endif // GAME_STATES_GAMESTATE_H


