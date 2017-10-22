#ifndef GAME_STATES_BASEMAPSTATE_H
#define GAME_STATES_BASEMAPSTATE_H

#include <flat.h>
#include "basestate.h"
#include "../mod/mod.h"
#include "../entity/entitypool.h"
#include "../entity/component/component.h"
#include "../entity/component/componentregistry.h"
#include "../entity/faction/faction.h"
#include "../debug/debugdisplay.h"
#include "../timer/timercontainer.h"

namespace game
{
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

class BaseMapState : public BaseState
{
	using Super = BaseState;
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

		flat::Vector2 getCursorMapPosition(game::Game& game, bool& isOnTile) const;

		void addFaction(const std::string& factionName);
		entity::faction::Faction* getFactionByName(const std::string& factionName);
		const entity::faction::Faction* getFactionByName(const std::string& factionName) const;

		std::shared_ptr<const entity::EntityTemplate> getEntityTemplate(game::Game& game, const std::string& entityTemplateName) const;
		std::shared_ptr<const map::TileTemplate> getTileTemplate(game::Game& game, const std::string& tileTemplateName) const;
		std::shared_ptr<const map::PropTemplate> getPropTemplate(game::Game& game, const std::string& propTemplateName) const;

		const entity::component::ComponentRegistry& getComponentRegistry() const { return m_componentRegistry; }

		entity::Entity* spawnEntityAtPosition(
			Game& game,
			const std::shared_ptr<const entity::EntityTemplate>& entityTemplate,
			const flat::Vector3& position,
			float heading = 0.f,
			float elevation = 0.f,
			entity::component::ComponentFlags componentFlags = entity::component::AllComponents,
			entity::component::ComponentFlags enabledComponentFlags = entity::component::AllComponents
		);
		void despawnEntity(entity::Entity* entity);
		void despawnEntityAtIndex(int index);
		void despawnEntities();

		inline const std::vector<entity::Entity*>& getSelectedEntities() const { return m_selectedEntities; }

		void setGhostTemplate(Game& game, const std::shared_ptr<const entity::EntityTemplate>& ghostTemplate);
		void clearGhostTemplate();

		entity::Entity* createEntity(
			Game& game,
			const std::shared_ptr<const entity::EntityTemplate>& entityTemplate,
			entity::component::ComponentFlags componentFlags = entity::component::AllComponents
		);
		void destroyEntity(entity::Entity* entity);

		void addEntityToMap(entity::Entity* entity);
		void removeEntityFromMap(entity::Entity* entity);
		entity::Entity* removeEntityFromMapAtIndex(int index);

		inline const flat::video::View& getGameView() const { return m_gameView; }

		bool isMouseOverUi(game::Game& game) const;
		
	protected:
		void update(game::Game& game) override;
		void addGhostEntity(game::Game& game);
		void removeGhostEntity(game::Game& game);
		void updateGameView(game::Game& game);
		void setCameraCenter(const flat::Vector3& cameraCenter);
		void setCameraZoom(float cameraZoom);
		void updateCameraView();
		
		void draw(game::Game& game) override;
		
		void buildUi(game::Game& game);

		virtual entity::component::ComponentFlags getComponentsFilter() const;

		bool isSelecting() const;
		void updateMouseOverEntity(Game& game);
		void clearMouseOverEntity();
		void setMouseOverColor(const entity::Entity* entity) const;
		void clearMouseOverColor(const entity::Entity* entity) const;
		bool updateSelectionWidget(Game& game);
		void selectClickedEntity(Game& game, const flat::Vector2& mousePosition, bool addToSelection);
		void selectEntitiesOfTypeInScreen(Game& game, const flat::Vector2& mousePosition, bool addToSelection);
		void updateSelectedEntities(Game& game, const flat::Vector2& bottomLeft, const flat::Vector2& topRight, bool addToSelection);
		void clearSelection();
		void addToSelectedEntities(entity::Entity* entity);
		void removeFromSelectedEntities(entity::Entity* entity);
		bool isSmallSelection() const;

#ifdef FLAT_DEBUG
		void copyStateBeforeReload(const BaseMapState& other);
		template <class T>
		void reloadToState(Game& game);
#endif
		
	protected:
		// resource loading
		flat::resource::ResourceManagerTimer<entity::EntityTemplate, Game&, const entity::component::ComponentRegistry&, std::string, std::string> m_entityTemplateManager;
		flat::resource::ResourceManager<map::TileTemplate, Game&, std::string> m_tileTemplateManager;
		flat::resource::ResourceManager<map::PropTemplate, Game&, std::string> m_propTemplateManager;
		
		// rendering settings
		flat::render::ProgramSettings m_entityRender;
		flat::render::ProgramSettings m_terrainRender;
		
		// level
		mod::Mod m_mod;

		std::map<std::string, entity::faction::Faction> m_factions;

		entity::component::ComponentRegistry m_componentRegistry;
		entity::EntityPool m_entityPool;

		std::vector<entity::Entity*> m_selectedEntities;
		entity::EntityHandle m_mouseOverEntity;
		const map::Tile* m_mouseOverTile;

		std::shared_ptr<const entity::EntityTemplate> m_ghostTemplate;
		entity::Entity* m_ghostEntity;
		
		flat::video::View m_gameView;
		flat::Vector2 m_cameraCenter2d;
		float m_cameraZoom;
		
		// ui
		flat::Vector2 m_mouseDownPosition;
		std::shared_ptr<flat::sharp::ui::Widget> m_selectionWidget;

#ifdef FLAT_DEBUG
		debug::DebugDisplay m_debugDisplay;

		bool m_isReloading;
#endif
};

template <class MapType>
class BaseMapStateImpl : public BaseMapState
{
	public:
		map::Map& getMap() override { return m_map; }

	protected:
		MapType m_map;
};

#ifdef FLAT_DEBUG
template<class T>
inline void BaseMapState::reloadToState(Game& game)
{
	std::unique_ptr<T> newState = std::make_unique<T>();
	newState->copyStateBeforeReload(*this);
	game.getStateMachine().setNextState(std::move(newState));
}
#endif

} // states
} // game

#endif // GAME_STATES_BASEMAPSTATE_H



