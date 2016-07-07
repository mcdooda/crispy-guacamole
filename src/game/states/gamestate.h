#ifndef GAME_STATES_GAMESTATE_H
#define GAME_STATES_GAMESTATE_H

#include <flat.h>
#include <lua5.2/lua.hpp>
#include "../mod/mod.h"
#include "../map/map.h"
#include "../map/displaymanager.h"

namespace game
{
class Game;
namespace states
{

class GameState : public flat::state::State
{
	public:
		void enter(flat::state::Agent* agent) override;
		void execute(flat::state::Agent* agent) override;
		void exit(flat::state::Agent* agent) override;
		
	private:
		void update(game::Game* game);
		void updateGameView(game::Game* game);
		void setCameraCenter(const flat::geometry::Vector3& cameraCenter);
		
		void draw(game::Game* game);
		
	private:
		// lua state
		lua_State* m_luaState;
		
		// rendering settings
		flat::video::Program m_program;
		flat::util::RenderSettings m_programRenderSettings;
		
		// level
		mod::Mod m_mod;
		map::Map m_map;
		map::DisplayManager m_mapDisplayManager;
		
		flat::video::View m_gameView;
};

} // states
} // game

#endif // GAME_STATES_GAMESTATE_H


