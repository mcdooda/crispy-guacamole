#ifndef GAME_STATES_BASEMAPSTATE_H
#define GAME_STATES_BASEMAPSTATE_H

#include <flat.h>
#include <lua5.2/lua.hpp>
#include "../mod/mod.h"
#include "../map/displaymanager.h"
#include "../entity/entitypool.h"
#include "../entity/component/component.h"
#include "../entity/component/componentregistry.h"
#include "../entity/faction/faction.h"
#include "../debug/debugdisplay.h"

namespace game
{
class Game;
namespace entity
{
class EntityTemplate;
}
namespace map
{
class Map;
class TileTemplate;
class PropTemplate;
}
namespace states
{

class BaseMapState : public flat::state::StateImpl<Game>
{
	public:
		BaseMapState();

		void enter(Game& game) override;
		void execute(Game& game) override;
		void exit(Game& game) override;
		
		void setModPath(const std::string& modPath);

		bool loadMap(Game& game, const std::string& mapName);
		bool saveMap(Game& game) const;

		virtual map::Map& getMap() = 0;
		const map::Map& getMap() const;

		flat::Vector2 getCursorMapPosition(game::Game& game);

		void addFaction(const std::string& factionName);
		entity::faction::Faction* getFactionByName(const std::string& factionName);
		const entity::faction::Faction* getFactionByName(const std::string& factionName) const;

		std::shared_ptr<const entity::EntityTemplate> getEntityTemplate(game::Game& game, const std::string& entityTemplateName) const;
		std::shared_ptr<const map::TileTemplate> getTileTemplate(game::Game& game, const std::string& tileTemplateName) const;
		std::shared_ptr<const map::PropTemplate> getPropTemplate(game::Game& game, const std::string& propTemplateName) const;

		const entity::component::ComponentRegistry& getComponentRegistry() const { return m_componentRegistry; }

		entity::Entity* spawnEntityAtPosition(Game& game, const std::shared_ptr<const entity::EntityTemplate>& entityTemplate, const flat::Vector3& position, float heading, float elevation);
		void despawnEntity(entity::Entity* entity);
		void despawnEntities();

		inline const std::vector<entity::Entity*>& getSelectedEntities() const { return m_selectedEntities; }
		
	protected:
		void update(game::Game& game);
		void updateGameView(game::Game& game);
		void setCameraCenter(const flat::Vector3& cameraCenter);
		void setCameraZoom(float cameraZoom);
		void updateCameraView();
		
		void draw(game::Game& game);
		
		void buildUi(game::Game& game);
		void updateUi(game::Game& game);
		void drawUi(game::Game& game);

		void resetViews(game::Game& game);

		virtual entity::component::ComponentFlags getComponentsFilter() const;

		inline bool isSelecting() { return !m_selectionWidget->getParent().expired(); }
		bool updateSelectionWidget(Game& game);
		void updateSelectedEntities(Game& game, const flat::Vector2& bottomLeft, const flat::Vector2& topRight, bool addToSelection);
		void clearSelection(Game& game);
		
	protected:
		// lua state
		lua_State* m_luaState;
		
		// resource loading
		flat::resource::ResourceManager<entity::EntityTemplate, Game&, lua_State*, const entity::component::ComponentRegistry&, std::string, std::string> m_entityTemplateManager;
		flat::resource::ResourceManager<map::TileTemplate, Game&, lua_State*, std::string> m_tileTemplateManager;
		flat::resource::ResourceManager<map::PropTemplate, Game&, lua_State*, std::string> m_propTemplateManager;
		
		// rendering settings
		flat::video::Program m_spriteProgram;
		flat::render::RenderSettings m_spriteProgramRenderSettings;
		
		flat::video::Program m_uiProgram;
		flat::render::RenderSettings m_uiProgramRenderSettings;
		
		// level
		mod::Mod m_mod;
		map::DisplayManager m_mapDisplayManager;

		std::map<std::string, entity::faction::Faction> m_factions;

		entity::component::ComponentRegistry m_componentRegistry;
		entity::EntityPool m_entityPool;

		std::vector<entity::Entity*> m_entities;
		std::vector<entity::Entity*> m_selectedEntities;
		
		flat::video::View m_gameView;
		flat::Vector2 m_cameraCenter2d;
		float m_cameraZoom;
		
		// ui
		std::shared_ptr<flat::sharp::ui::RootWidget> m_ui;

		flat::Vector2 m_mouseDownPosition;
		std::shared_ptr<flat::sharp::ui::Widget> m_selectionWidget;

		FLAT_DEBUG_ONLY(debug::DebugDisplay m_debugDisplay;)
};

template <class MapType>
class BaseMapStateImpl : public BaseMapState
{
	public:
		map::Map& getMap() override { return m_map; }

	protected:
		MapType m_map;
};

} // states
} // game

#endif // GAME_STATES_BASEMAPSTATE_H



