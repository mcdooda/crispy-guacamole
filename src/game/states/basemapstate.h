#ifndef GAME_STATES_BASEMAPSTATE_H
#define GAME_STATES_BASEMAPSTATE_H

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

class BaseMapState : public flat::state::State
{
	public:
		void enter(flat::state::Agent* agent) override;
		void execute(flat::state::Agent* agent) override;
		void exit(flat::state::Agent* agent) override;
		
		void setModPath(const std::string& modPath);
		
	protected:
		void update(game::Game* game);
		void updateGameView(game::Game* game);
		void setCameraCenter(const flat::geometry::Vector3& cameraCenter);
		
		void draw(game::Game* game);
		
		void buildUi(game::Game* game);
		void updateUi(game::Game* game);
		void drawUi(game::Game* game);
		
	protected:
		// lua state
		lua_State* m_luaState;
		
		// rendering settings
		flat::video::Program m_spriteProgram;
		flat::util::RenderSettings m_spriteProgramRenderSettings;
		
		flat::video::Program m_uiProgram;
		flat::util::RenderSettings m_uiProgramRenderSettings;
		
		// level
		mod::Mod m_mod;
		map::Map m_map;
		map::DisplayManager m_mapDisplayManager;
		
		flat::video::View m_gameView;
		
		// ui
		std::unique_ptr<flat::sharp::ui::RootWidget> m_ui;
};

} // states
} // game

#endif // GAME_STATES_BASEMAPSTATE_H



