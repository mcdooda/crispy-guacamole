#ifndef GAME_STATES_GAMESTATE_H
#define GAME_STATES_GAMESTATE_H

#include <flat.h>
#include "basemapstate.h"
#include "../map/map.h"

namespace game
{
namespace states
{

class GameState : public BaseMapStateImpl<map::Map>
{
	using Super = BaseMapStateImpl<map::Map>;
	public:
		void enter(Game& game) override final;
		void execute(Game& game) override final;

		void setCanPlaceGhostEntity(flat::lua::UniqueLuaReference<LUA_TFUNCTION>&& canPlaceGhostEntity);
		void setOnGhostEntityPlaced(flat::lua::UniqueLuaReference<LUA_TFUNCTION>&& onGhostEntityPlaced);

		bool canPlaceGhostEntity(map::TileIndex tileIndex) const override;
		bool onGhostEntityPlaced(map::TileIndex tileIndex) override;

	protected:
		void startLevelScript(Game& game);
		void updateLevelScript();

#ifdef FLAT_DEBUG
		void handleDebugInputs(Game& game);
#endif

	private:
		flat::lua::Thread m_levelThread;

		flat::lua::UniqueLuaReference<LUA_TFUNCTION> m_canPlaceGhostEntity;
		flat::lua::UniqueLuaReference<LUA_TFUNCTION> m_onGhostEntityPlaced;
};

} // states
} // game

#endif // GAME_STATES_GAMESTATE_H


