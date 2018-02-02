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

	protected:
		void startLevelScript(Game& game);
		void updateLevelScript();

#ifdef FLAT_DEBUG
		void handleDebugInputs(Game& game);
#endif

	private:
		flat::lua::Thread m_levelThread;
};

} // states
} // game

#endif // GAME_STATES_GAMESTATE_H


