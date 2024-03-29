#ifndef GAME_STATES_BASEMAPSTATE_H
#define GAME_STATES_BASEMAPSTATE_H

#include <flat.h>

#include "basestate.h"

#include "mod/mod.h"

#include "entity/entity.h"
#include "entity/entitypool.h"
#include "entity/entityupdater.h"
#include "entity/component/component.h"
#include "entity/component/componentregistry.h"
#include "entity/faction/faction.h"

#include "debug/debugdisplay.h"

#include "map/map.h"
#include "map/displaymanager.h"

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

		bool loadMap(Game& game);
		bool saveMap(Game& game) const;

		map::Map& getMap();
		const map::Map& getMap() const;

		flat::Vector2 getCursorMapPosition(game::Game& game, bool& isOnTile) const;
		FLAT_DEBUG_ONLY(void debugCursorPosition(Game& game);)

		void addFaction(const std::string& factionName);
		entity::faction::Faction* getFactionByName(const std::string& factionName);
		const entity::faction::Faction* getFactionByName(const std::string& factionName) const;

		std::shared_ptr<const entity::EntityTemplate> getEntityTemplate(game::Game& game, const std::filesystem::path& entityTemplatePath) const;
		std::shared_ptr<const map::TileTemplate> getTileTemplate(game::Game& game, const std::filesystem::path& tileTemplatePath) const;
		std::shared_ptr<const map::PropTemplate> getPropTemplate(game::Game& game, const std::filesystem::path& propTemplatePath) const;

		const entity::component::ComponentRegistry& getComponentRegistry() const { return m_componentRegistry; }

		const entity::EntityUpdater& getEntityUpdater() const { return m_entityUpdater; }

		entity::Entity* spawnEntityAtPosition(
			Game& game,
			const std::shared_ptr<const entity::EntityTemplate>& entityTemplate,
			const flat::Vector3& position,
			float heading = 0.f,
			float elevation = 0.f,
			entity::Entity* instigator = nullptr,
			entity::component::ComponentFlags componentFlags = entity::component::AllComponents,
			entity::component::ComponentFlags enabledComponentFlags = entity::component::AllComponents
		);
		void despawnEntity(entity::Entity* entity);
		void despawnMarkedEntities();
		void despawnAllEntities();

		void setEntitySelected(Game& game, entity::Entity* entity, bool selected);
		inline const std::vector<entity::Entity*>& getSelectedEntities() const { return m_selectedEntities; }

		template <class Func>
		inline void eachEntityOfType(const std::shared_ptr<const entity::EntityTemplate>& entityTemplate, Func func) const;

		void setGhostTemplate(Game& game, const std::shared_ptr<const entity::EntityTemplate>& ghostTemplate);
		void clearGhostTemplate();
		entity::Entity* addGhostEntityAt(Game& game, const flat::Vector3& position);
		virtual std::vector<flat::Vector2> getGhostEntityPositions(const flat::Vector2& cursorPosition, map::TileIndex tileIndex) const;
		virtual bool onGhostEntityPlaced(map::TileIndex tileIndex, bool& continueAction);

		entity::Entity* createEntity(
			Game& game,
			const std::shared_ptr<const entity::EntityTemplate>& entityTemplate,
			entity::component::ComponentFlags componentFlags = entity::component::AllComponents
		);
		void destroyEntity(entity::Entity* entity);

		inline const flat::Vector2& getCameraCenter() const { return m_cameraCenter2d; }
		void setCameraCenter(const flat::Vector2& cameraCenter);
		void setCameraZoom(float cameraZoom);
		inline void lockCamera() { m_cameraLocked ++; }
		void unlockCamera();
		flat::Vector2 convertToCameraPosition(const flat::Vector3& position) const;

		bool addEntityToMap(entity::Entity* entity);
		void removeEntityFromMap(entity::Entity* entity);

		inline const flat::video::View& getGameView() const { return m_gameView; }

		bool isMouseOverUi(game::Game& game) const;

		int addSelectionChangedCallback(lua_State* L, int index);
		void removeSelectionChangedCallback(int index);

		// time
		inline const flat::time::Clock& getGameClock() const { FLAT_ASSERT(m_gameClock != nullptr); return *m_gameClock.get(); }
		inline flat::time::Clock& getGameClock() { FLAT_ASSERT(m_gameClock != nullptr); return *m_gameClock.get(); }

		inline const flat::lua::timer::TimerContainer& getGameTimerContainer() const { FLAT_ASSERT(m_gameTimerContainer != nullptr); return *m_gameTimerContainer.get(); }
		inline flat::lua::timer::TimerContainer& getGameTimerContainer() { FLAT_ASSERT(m_gameTimerContainer != nullptr); return *m_gameTimerContainer.get(); }

#ifdef FLAT_DEBUG
		void setGamePause(Game& game, bool pause, bool pauseNextFrame);
		inline bool isGamePaused() const { return m_gamePaused; }
#endif

	protected:
		void update(game::Game& game) override;
		std::vector<entity::Entity*> addGhostEntities(game::Game& game);
		void setCameraCenter(const flat::Vector3& cameraCenter);
		void updateGameView(game::Game& game);
		void updateCameraView();

		void initLua(Game& game) override;

		void draw(game::Game& game) override;

		void buildUi(game::Game& game);

		virtual entity::component::ComponentFlags getComponentsFilter() const;

		bool isSelecting() const;
		void updateMouseOverEntity(Game& game);
		void clearMouseOverEntity();
		bool updateSelectionWidget(Game& game);
		void selectClickedEntity(Game& game, const flat::Vector2& mousePosition, bool addToSelection);
		void selectEntitiesOfTypeInScreen(Game& game, const flat::Vector2& mousePosition, bool addToSelection);
		void getEntitiesInSelection(const flat::Vector2& bottomLeft, const flat::Vector2& topRight, std::vector<entity::Entity*>& entities) const;
		void updateSelectedEntities(Game& game, const flat::Vector2& bottomLeft, const flat::Vector2& topRight, bool addToSelection);
		void clearSelection();
		bool addToSelectedEntities(Game& game, entity::Entity* entity);
		void removeFromSelectedEntities(entity::Entity* entity);
		bool isSmallSelection() const;
		void clickEntity(entity::Entity* entity) const;
		bool forceEntitySelection(Game& game) const;

		bool findAssetForBackwardCompatibility(
			Game& game,
			const std::string& assetType,
			const std::filesystem::path& path,
			const flat::tool::Asset*& asset) const;

		// game features
		void handleGameActionInputs(Game& game);
		void moveToFormation(Game& game);
		void updateEntities();
		void updateMap(Game& game);

#ifdef FLAT_DEBUG
		void copyStateBeforeReload(const BaseMapState& other);
		template <class T>
		void reloadToState(Game& game);
#endif

	public:
		flat::Slot<> selectionChanged;

	protected:
		// resource loading
		flat::resource::StrongResourceManager<entity::EntityTemplate, std::string, Game&, const entity::component::ComponentRegistry&> m_entityTemplateManager;
		flat::resource::StrongResourceManager<map::TileTemplate, std::string, Game&> m_tileTemplateManager;
		flat::resource::StrongResourceManager<map::PropTemplate, std::string, Game&> m_propTemplateManager;

		// rendering settings
		flat::render::ProgramSettings m_entityRender;
		flat::render::ProgramSettings m_terrainRender;

		// level
		map::Map m_map;

		std::map<std::string, entity::faction::Faction> m_factions;

		entity::component::ComponentRegistry m_componentRegistry;
		entity::EntityPool m_entityPool;
		entity::EntityUpdater m_entityUpdater;
		map::DisplayManager m_displayManager;

		std::vector<entity::Entity*> m_selectedEntities;
		flat::lua::SlotProxy<> m_selectionChangedSlotProxy;

		std::vector<entity::EntityHandle> m_entitiesInSelection;

		entity::EntityHandle m_mouseOverEntity;
		map::TileIndex m_mouseOverTileIndex;

		std::shared_ptr<const entity::EntityTemplate> m_ghostTemplate;

		flat::video::View m_gameView;
		flat::Vector2 m_cameraCenter2d;
		float m_cameraZoom;
		int m_cameraLocked;

		// time
		std::shared_ptr<flat::time::Clock> m_gameClock;
		std::shared_ptr<flat::lua::timer::TimerContainer> m_gameTimerContainer;

		// ui
		std::shared_ptr<flat::input::context::InputContext> m_gameInputContext;
		flat::Vector2 m_mouseDownPosition;
		std::shared_ptr<flat::sharp::ui::Widget> m_selectionWidget;

#ifdef FLAT_DEBUG
		debug::DebugDisplay m_debugDisplay;

		bool m_isReloading : 1;

		bool m_gamePaused : 1;
		bool m_pauseNextFrame : 1;
#endif
};

template <class Func>
void BaseMapState::eachEntityOfType(const std::shared_ptr<const entity::EntityTemplate>& entityTemplate, Func func) const
{
	const std::vector<entity::Entity*>& entities = m_entityUpdater.getEntities();
	for (entity::Entity* entity : entities)
	{
		if (entity->getEntityTemplate() == entityTemplate)
		{
			func(entity);
		}
	}
}

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



