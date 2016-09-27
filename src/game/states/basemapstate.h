#ifndef GAME_STATES_BASEMAPSTATE_H
#define GAME_STATES_BASEMAPSTATE_H

#include <flat.h>
#include <lua5.2/lua.hpp>
#include "../mod/mod.h"
#include "../map/map.h"
#include "../map/displaymanager.h"
#include "../entity/entitypool.h"

namespace game
{
class Game;
namespace entity
{
class EntityTemplate;
}
namespace map
{
class TileTemplate;
class PropTemplate;
}
namespace states
{

class BaseMapState : public flat::state::State
{
	public:
		void enter(flat::state::Agent* agent) override;
		void execute(flat::state::Agent* agent) override;
		void exit(flat::state::Agent* agent) override;
		
		void setModPath(const std::string& modPath);

		bool loadMap(Game* game, const std::string& mapName);
		bool saveMap(Game* game) const;

		inline map::Map& getMap() { return m_map; }
		inline const map::Map& getMap() const { return m_map; }

		flat::Vector2 getCursorMapPosition(game::Game* game);

		std::shared_ptr<const entity::EntityTemplate> getEntityTemplate(game::Game* game, const std::string& entityTemplateName) const;
		std::shared_ptr<const map::TileTemplate> getTileTemplate(game::Game* game, const std::string& tileTemplateName) const;
		std::shared_ptr<const map::PropTemplate> getPropTemplate(game::Game* game, const std::string& propTemplateName) const;

		entity::Entity* spawnEntityAtPosition(const std::shared_ptr<const entity::EntityTemplate>& entityTemplate, const flat::Vector3& position);
		
	protected:
		void update(game::Game* game);
		void updateGameView(game::Game* game);
		void setCameraCenter(const flat::Vector3& cameraCenter);
		void setCameraZoom(float cameraZoom);
		void updateCameraView();
		
		void draw(game::Game* game);
		
		void buildUi(game::Game* game);
		void updateUi(game::Game* game);
		void drawUi(game::Game* game);
		
	protected:
		// lua state
		lua_State* m_luaState;
		
		// resource loading
		flat::resource::ResourceManager<entity::EntityTemplate, Game*, lua_State*, std::string> m_entityTemplateManager;
		flat::resource::ResourceManager<map::TileTemplate, Game*, lua_State*, std::string> m_tileTemplateManager;
		flat::resource::ResourceManager<map::PropTemplate, Game*, lua_State*, std::string> m_propTemplateManager;
		
		// rendering settings
		flat::video::Program m_spriteProgram;
		flat::util::RenderSettings m_spriteProgramRenderSettings;
		
		flat::video::Program m_uiProgram;
		flat::util::RenderSettings m_uiProgramRenderSettings;
		
		// level
		mod::Mod m_mod;
		map::Map m_map;
		map::DisplayManager m_mapDisplayManager;
		entity::EntityPool m_entityPool;
		std::vector<entity::Entity*> m_entities;
		
		flat::video::View m_gameView;
		flat::Vector2 m_cameraCenter2d;
		float m_cameraZoom;
		
		// ui
		std::unique_ptr<flat::sharp::ui::RootWidget> m_ui;
};

} // states
} // game

#endif // GAME_STATES_BASEMAPSTATE_H



