#include <flat.h>
#include "basemapstate.h"
#include "selectmapstate.h"
#include "mapeditor/lua/editor.h"
#include "../game.h"
#include "../timer/lua/timer.h"
#include "../map/map.h"
#include "../map/tile.h"
#include "../map/tiletemplate.h"
#include "../map/proptemplate.h"
#include "../map/lua/map.h"
#include "../map/lua/zone.h"
#include "../map/brush/lua/brush.h"
#include "../entity/lua/entity.h"
#include "../entity/component/lua/componentregistry.h"
#include "../entity/component/components/attack/attackcomponent.h"
#include "../entity/component/components/behavior/behaviorcomponent.h"
#include "../entity/component/components/collision/collisioncomponent.h"
#include "../entity/component/components/selection/selectioncomponent.h"
#include "../entity/faction/lua/faction.h"
#include "../entity/entitytemplate.h"

namespace game
{
namespace states
{

BaseMapState::BaseMapState() :
	m_entityPool(m_componentRegistry),
	m_entityUpdater(m_componentRegistry)
#ifdef FLAT_DEBUG
	, m_isReloading(false)
#endif
{

}

void BaseMapState::enter(Game& game)
{
	Super::enter(game);

#ifdef FLAT_DEBUG
	m_gamePaused = false;
	m_pauseNextFrame = false;
#endif
	
	// init lua then
	initLua(game);
	lua_State* L = game.lua->state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		entity::lua::open(game);
		entity::component::lua::open(L, m_componentRegistry);
		entity::faction::lua::open(L, m_mod.getFactionsConfigPath());
		map::lua::map::open(L);
		map::lua::zone::open(game);
		map::brush::lua::open(game);
		editor::lua::open(L);

		flat::lua::doFile(L, "data/common/init.lua");
	}
	
	// ui
	buildUi(game);
	
	// rendering settings
	m_entityRender.program.load("data/shaders/sprite/entityspritebatch.frag", "data/shaders/sprite/entityspritebatch.vert");
	
	m_entityRender.settings.textureUniform              = m_entityRender.program.getUniform<flat::video::Texture>("objectTexture");
	m_entityRender.settings.viewProjectionMatrixUniform = m_entityRender.program.getUniform<flat::Matrix4>("vpMatrix");

	m_entityRender.settings.positionAttribute           = m_entityRender.program.getAttribute("position");
	m_entityRender.settings.uvAttribute                 = m_entityRender.program.getAttribute("uv");
	m_entityRender.settings.colorAttribute              = m_entityRender.program.getAttribute("color");
	m_entityRender.settings.normalAttribute             = m_entityRender.program.getAttribute("normal");

	entity::Entity::setEntityProgramSettings(m_entityRender);

	m_terrainRender.program.load("data/shaders/sprite/terrainspritebatch.frag", "data/shaders/sprite/terrainspritebatch.vert");

	m_terrainRender.settings.textureUniform              = m_terrainRender.program.getUniform<flat::video::Texture>("objectTexture");
	m_terrainRender.settings.viewProjectionMatrixUniform = m_terrainRender.program.getUniform<flat::Matrix4>("vpMatrix");

	m_terrainRender.settings.positionAttribute           = m_terrainRender.program.getAttribute("position");
	m_terrainRender.settings.uvAttribute                 = m_terrainRender.program.getAttribute("uv");
	m_terrainRender.settings.colorAttribute              = m_terrainRender.program.getAttribute("color");
	m_terrainRender.settings.normalAttribute             = m_terrainRender.program.getAttribute("normal");

	map::Tile::setTileProgramSettings(m_terrainRender);
	
	initRender(game);

	m_entityTemplateManager.init(game);
	
	loadMap(game, game.mapName);

	// load debug display resources *after* the map is loaded!
	FLAT_DEBUG_ONLY(m_debugDisplay.loadResources(game);)

		// reset view
#ifdef FLAT_DEBUG
	if (!m_isReloading)
	{
#endif
		m_gameView.setWindow(game.video->window);
		m_gameView.updateProjection();
		m_cameraZoom = 1.f;

		map::Map& map = getMap();
		int minX, maxX, minY, maxY;
		map.getBounds(minX, maxX, minY, maxY);
		flat::Vector3 cameraCenter((maxX + minX) / 2.f, (maxY + minY) / 2.f, 0.f);
		setCameraCenter(cameraCenter);
#ifdef FLAT_DEBUG
	}
#endif

	resetViews(game);

	m_mouseOverEntity = nullptr;
	m_mouseOverTile = nullptr;
	m_ghostEntity = nullptr;
}

void BaseMapState::execute(Game& game)
{
	const auto& input = game.input;

	if (input->window->isResized())
	{
		resetViews(game);
	}

	if (input->keyboard->isJustPressed(K(ESCAPE)))
	{
		std::unique_ptr<SelectMapState> selectMapState = std::make_unique<SelectMapState>();
		game.getStateMachine().setNextState(std::move(selectMapState));
	}
	
	update(game);

	clearScreen(game);
	draw(game);
}

void BaseMapState::exit(Game& game)
{
	map::Map& map = getMap();
	std::vector<entity::Entity*> entities = m_entityUpdater.getEntities();
	for (entity::Entity* entity : entities)
	{
		despawnEntity(entity);
	}
	clearGhostTemplate();

	Super::exit(game);
}

void BaseMapState::setModPath(const std::string& modPath)
{
	m_mod.setPath(modPath);
}

bool BaseMapState::loadMap(Game& game, const std::string& mapName)
{
	map::Map& map = getMap();
	map.setDisplayManager(&m_displayManager);
	if (map.load(game, m_mod, mapName))
	{
		game.mapName = mapName;
		return true;
	}
	return false;
}

bool BaseMapState::saveMap(Game& game) const
{
	const map::Map& map = getMap();
	return map.save(m_mod, game.mapName, m_entityUpdater.getEntities());
}

const map::Map& BaseMapState::getMap() const
{
	return const_cast<BaseMapState*>(this)->getMap();
}

flat::Vector2 BaseMapState::getCursorMapPosition(game::Game& game, bool& isOnTile) const
{
	const flat::Vector2& cursorPosition = game.input->mouse->getPosition();
	flat::Vector2 gameViewPosition = m_gameView.getRelativePosition(cursorPosition);

	const map::Map& map = getMap();
	const flat::Vector2& xAxis = map.getXAxis();
	const flat::Vector2& yAxis = map.getYAxis();

	auto gameViewToMap = [&xAxis, &yAxis](const flat::Vector2& screenPosition)
	{
		return flat::Vector2(
			(screenPosition.x * yAxis.y - screenPosition.y * yAxis.x) / (xAxis.x * yAxis.y - xAxis.y * yAxis.x),
			(screenPosition.y * xAxis.x - screenPosition.x * xAxis.y) / (yAxis.y * xAxis.x - yAxis.x * xAxis.y)
		);
	};

	flat::Vector2 mapPosition = gameViewToMap(gameViewPosition);
	isOnTile = false;

	if (m_mouseOverTile != nullptr)
	{
		const flat::Vector2& spritePosition = m_mouseOverTile->getSprite().getPosition();
		flat::Vector2 delta = gameViewToMap(gameViewPosition - spritePosition);

		flat::Vector2 tileCenter = flat::Vector2(m_mouseOverTile->getX(), m_mouseOverTile->getY());
		flat::Vector2 mapPositionOnTile = tileCenter + delta;
		if (mapPositionOnTile.x <= tileCenter.x + 0.5f && mapPositionOnTile.y <= tileCenter.y + 0.5f)
		{
			mapPosition.x = glm::clamp(mapPositionOnTile.x, tileCenter.x - (0.5f - flat::EPSILON), tileCenter.x + (0.5f - flat::EPSILON));
			mapPosition.y = glm::clamp(mapPositionOnTile.y, tileCenter.y - (0.5f - flat::EPSILON), tileCenter.y + (0.5f - flat::EPSILON));
			isOnTile = true;
		}
		else
		{
			flat::Vector2 adjacentTilePosition;
			if (mapPositionOnTile.x - tileCenter.x > mapPositionOnTile.y - tileCenter.y)
			{
				adjacentTilePosition = flat::Vector2(tileCenter.x + 0.5f + flat::EPSILON, tileCenter.y);
			}
			else
			{
				adjacentTilePosition = flat::Vector2(tileCenter.x, tileCenter.y + 0.5f + flat::EPSILON);
			}

			const map::Tile* adjacentTile = map.getTileIfExists(adjacentTilePosition.x, adjacentTilePosition.y);
			if (adjacentTile != nullptr)
			{
				mapPosition = adjacentTilePosition;
				isOnTile = true;
			}
			else
			{
				mapPosition.x = std::min(mapPositionOnTile.x, tileCenter.x + 0.5f + flat::EPSILON);
				mapPosition.y = std::min(mapPositionOnTile.y, tileCenter.y + 0.5f + flat::EPSILON);
			}
		}
	}

	return mapPosition;
}

#ifdef FLAT_DEBUG
void BaseMapState::debugCursorPosition(Game& game)
{
	bool cursorOnTile;
	flat::Vector2 position2d = getCursorMapPosition(game, cursorOnTile);
	flat::Vector3 position3d(position2d, 0.f);
	flat::video::Color color = flat::video::Color::RED;
	const map::Tile* tile = getMap().getTileIfExists(position2d.x, position2d.y);
	if (tile != nullptr && cursorOnTile)
	{
		position3d.z = tile->getZ();
		color = flat::video::Color::BLUE;

		{
			flat::Vector3 position3d(tile->getX(), tile->getY(), tile->getZ());
			m_debugDisplay.add3dLine(position3d + flat::Vector3(-0.5f, -0.5f, 0.f), position3d + flat::Vector3(0.5f, -0.5f, 0.f));
			m_debugDisplay.add3dLine(position3d + flat::Vector3(0.5f, -0.5f, 0.f), position3d + flat::Vector3(0.5f, 0.5f, 0.f));
			m_debugDisplay.add3dLine(position3d + flat::Vector3(0.5f, 0.5f, 0.f), position3d + flat::Vector3(-0.5f, 0.5f, 0.f));
			m_debugDisplay.add3dLine(position3d + flat::Vector3(-0.5f, 0.5f, 0.f), position3d + flat::Vector3(-0.5f, -0.5f, 0.f));
		}
	}
	m_debugDisplay.add3dCircle(position3d, 0.1f, color, 1.f);
}
#endif

void BaseMapState::addFaction(const std::string& factionName)
{
	m_factions.emplace(std::piecewise_construct,
		std::forward_as_tuple(factionName),
		std::forward_as_tuple(factionName));
}

entity::faction::Faction* BaseMapState::getFactionByName(const std::string& factionName)
{
	std::map<std::string, entity::faction::Faction>::iterator it = m_factions.find(factionName);
	if (it == m_factions.end())
	{
		return nullptr;
	}
	return &it->second;
}

const entity::faction::Faction* BaseMapState::getFactionByName(const std::string& factionName) const
{
	std::map<std::string, entity::faction::Faction>::const_iterator it = m_factions.find(factionName);
	if (it == m_factions.end())
	{
		return nullptr;
	}
	return &it->second;
}

std::shared_ptr<const entity::EntityTemplate> BaseMapState::getEntityTemplate(game::Game& game, const std::string& entityTemplateName) const
{
	std::string entityTemplatePath = m_mod.getEntityTemplatePath(entityTemplateName);
	return m_entityTemplateManager.getResource(30.f, game, m_componentRegistry, entityTemplatePath, entityTemplateName);
}

std::shared_ptr<const map::TileTemplate> BaseMapState::getTileTemplate(game::Game& game, const std::string& tileTemplateName) const
{
	std::string tileTemplatePath = m_mod.getTileTemplatePath(tileTemplateName);
	return m_tileTemplateManager.getResource(game, tileTemplatePath);
}

std::shared_ptr<const map::PropTemplate> BaseMapState::getPropTemplate(game::Game& game, const std::string& propTemplateName) const
{
	std::string propTemplatePath = m_mod.getPropTemplatePath(propTemplateName);
	return m_propTemplateManager.getResource(game, propTemplatePath);
}

entity::Entity* BaseMapState::spawnEntityAtPosition(
	Game& game,
	const std::shared_ptr<const entity::EntityTemplate>& entityTemplate,
	const flat::Vector3& position,
	float heading,
	float elevation,
	entity::component::ComponentFlags componentFlags,
	entity::component::ComponentFlags enabledComponentFlags
)
{
	entity::Entity* entity = createEntity(game, entityTemplate, componentFlags);

	// disable components not in enabledComponentFlags
	if (enabledComponentFlags != entity::component::AllComponents)
	{
		for (entity::component::Component* component : entity->getComponents())
		{
			entity::component::ComponentFlags componentFlag = component->getComponentType().getComponentTypeFlag();
			if ((enabledComponentFlags & componentFlag) == 0)
			{
				component->incDisableLevel();
			}
		}
	}

	entity->setPosition(position);
	entity->setHeading(heading);
	entity->setElevation(elevation);
	addEntityToMap(entity);
	return entity;
}

void BaseMapState::despawnEntity(entity::Entity* entity)
{
	removeEntityFromMap(entity);
	removeFromSelectedEntities(entity);
	destroyEntity(entity);
}

void BaseMapState::despawnEntities()
{
	std::vector<entity::Entity*> entities = m_entityUpdater.getEntities();
	for (entity::Entity* entity : entities)
	{
		if (entity->isMarkedForDelete())
		{
			despawnEntity(entity);
		}
	}
}

void BaseMapState::setGhostTemplate(Game& game, const std::shared_ptr<const entity::EntityTemplate>& ghostTemplate)
{
	clearGhostTemplate();
	m_ghostTemplate = ghostTemplate;
	using namespace entity::component;
	ComponentFlags componentFlags = AllComponents;
	componentFlags &= ~attack::AttackComponent::getFlag();
	componentFlags &= ~behavior::BehaviorComponent::getFlag();
	componentFlags &= ~collision::CollisionComponent::getFlag();
	m_ghostEntity = createEntity(game, ghostTemplate, componentFlags);
}

void BaseMapState::clearGhostTemplate()
{
	if (m_ghostEntity != nullptr)
	{
		destroyEntity(m_ghostEntity);
	}
	m_ghostTemplate.reset();
	m_ghostEntity = nullptr;
}

bool BaseMapState::canPlaceGhostEntity(const map::Tile* tile) const
{
	return true;
}

bool BaseMapState::onGhostEntityPlaced()
{
	return true;
}

entity::Entity* BaseMapState::createEntity(Game& game, const std::shared_ptr<const entity::EntityTemplate>& entityTemplate, entity::component::ComponentFlags componentFlags)
{
	entity::component::ComponentFlags componentsFilter = getComponentsFilter() & componentFlags;
	entity::Entity* entity = m_entityPool.createEntity(entityTemplate, m_componentRegistry, componentsFilter);
	return entity;
}

void BaseMapState::destroyEntity(entity::Entity* entity)
{
	FLAT_ASSERT(entity->getMap() == nullptr);
	m_entityPool.destroyEntity(entity);
}

void BaseMapState::addEntityToMap(entity::Entity* entity)
{
	FLAT_ASSERT(entity->getMap() == nullptr);
	m_entityUpdater.registerEntity(entity);
	entity->onAddedToMap(&getMap());
	flat::time::Clock& clock = getClock();
	m_displayManager.addEntity(entity);
	m_entityUpdater.updateSingleEntity(entity, clock.getTime(), clock.getDT());
}

void BaseMapState::removeEntityFromMap(entity::Entity* entity)
{
	FLAT_ASSERT(entity->getMap() != nullptr);
	m_entityUpdater.unregisterEntity(entity);
	entity->onRemovedFromMap();
	m_displayManager.removeEntity(entity);
}

bool BaseMapState::isMouseOverUi(game::Game& game) const
{
	flat::sharp::ui::RootWidget* root = game.ui->root.get();
	return root->isMouseOver() && root->getCurrentMouseOverWidget().lock() != m_selectionWidget;
}

#ifdef FLAT_DEBUG
void BaseMapState::setGamePause(Game& game, bool pause, bool pauseNextFrame)
{
	m_pauseNextFrame = pauseNextFrame;

	if (m_gamePaused == pause)
	{
		return;
	}

	m_gamePaused = pause;

	flat::time::Clock& clock = getClock();
	if (pause)
	{
		clock.pause();
	}
	else
	{
		clock.resume();
	}
}
#endif

void BaseMapState::update(game::Game& game)
{
	updateGameView(game);
	m_entityTemplateManager.update();
	Super::update(game);

	//debugCursorPosition(game);
}

void BaseMapState::addGhostEntity(game::Game& game)
{
	if (m_ghostEntity != nullptr && !isMouseOverUi(game) && !isSelecting() && !m_mouseOverEntity.isValid())
	{
		bool isOnTile;
		flat::Vector2 cursorPosition = getCursorMapPosition(game, isOnTile);
		if (isOnTile)
		{
			const map::Tile* tile = getMap().getTileIfWalkable(cursorPosition.x, cursorPosition.y);
			if (tile != nullptr)
			{
				m_ghostEntity->resetComponents();
				m_ghostEntity->setHeading(0.f);
				m_ghostEntity->setElevation(0.f);
				flat::Vector3 ghostPosition(cursorPosition, tile->getZ());
				m_ghostEntity->setPosition(ghostPosition);
				addEntityToMap(m_ghostEntity);
				flat::render::Sprite& sprite = m_ghostEntity->getSprite();
				flat::video::Color color;
				if (canPlaceGhostEntity(tile))
				{
					color = flat::video::Color::WHITE;
					color.a = 0.6f;
				}
				else
				{
					color = flat::video::Color::BLACK;
				}
				sprite.setColor(color);

#ifdef FLAT_DEBUG
				m_ghostEntity->debugDraw(m_debugDisplay);
#endif
			}
		}
	}
}

void BaseMapState::removeGhostEntity(game::Game & game)
{
	if (m_ghostEntity != nullptr && m_ghostEntity->getMap() != nullptr)
	{
		removeEntityFromMap(m_ghostEntity);
	}
}

void BaseMapState::updateGameView(game::Game& game)
{
	const auto& keyboard = game.input->keyboard;
	const auto& mouse = game.input->mouse;
	
	const map::Map& map = getMap();
	const flat::Vector2& xAxis = map.getXAxis();
	flat::Vector2 speed(-xAxis.x, xAxis.y);
	
	flat::Vector2 move;
	
	// keyboard wins over mouse
	bool leftPressed = keyboard->isPressed(K(LEFT));
	bool rightPressed = keyboard->isPressed(K(RIGHT));
	bool upPressed = keyboard->isPressed(K(UP));
	bool downPressed = keyboard->isPressed(K(DOWN));
	
	if (leftPressed && !rightPressed)
		move.x = -speed.x;
	else if (rightPressed && !leftPressed)
		move.x = speed.x;
		
	if (upPressed && !downPressed)
		move.y = -speed.y;
	else if (downPressed && !upPressed)
		move.y = speed.y;
	
	const float cameraSpeed = 40.f;
	m_cameraCenter2d += move * m_uiClock->getDT() * cameraSpeed;
	updateCameraView();

	if (mouse->wheelJustMoved() && keyboard->isPressed(K(LCTRL)))
	{
		const float zoom = m_cameraZoom * static_cast<float>(std::pow(2, mouse->getWheelMove().y));
		setCameraZoom(zoom);
	}
	
	if (game.input->window->isResized())
	{
		m_gameView.updateProjection();
	}
}

void BaseMapState::setCameraCenter(const flat::Vector3& cameraCenter)
{
	const map::Map& map = getMap();
	m_cameraCenter2d = flat::Vector2(map.getTransform() * cameraCenter);
	updateCameraView();
}

void BaseMapState::setCameraZoom(float cameraZoom)
{
	m_cameraZoom = cameraZoom;
	updateCameraView();
}

void BaseMapState::updateCameraView()
{
	m_gameView.reset();
	m_gameView.flipY();
	m_gameView.zoom(m_cameraZoom);
	m_gameView.move(m_cameraCenter2d);
}

void BaseMapState::draw(game::Game& game)
{
	// map
	map::Map& map = getMap();
	map.updateTilesNormals();
	addGhostEntity(game);
	m_displayManager.sortByDepthAndDraw(game, m_gameView);
	removeGhostEntity(game);
	
#ifdef FLAT_DEBUG
	m_entityUpdater.debugDraw(m_debugDisplay);
	m_debugDisplay.drawElements(game, m_gameView);
#endif
	
	Super::draw(game);
}

void BaseMapState::buildUi(game::Game& game)
{
	// create selection widget
	flat::sharp::ui::WidgetFactory& widgetFactory = game.ui->factory;
	m_selectionWidget = widgetFactory.makeFixedSize(flat::Vector2(1.f, 1.f));
	m_selectionWidget->setPositionPolicy(flat::sharp::ui::Widget::PositionPolicy::BOTTOM_LEFT);
	m_selectionWidget->setBackgroundColor(flat::video::Color(0.f, 8.f, 0.f, 0.3f));
}

entity::component::ComponentFlags BaseMapState::getComponentsFilter() const
{
	return entity::component::AllComponents;
}

bool BaseMapState::isSelecting() const
{
	flat::sharp::ui::Widget* selectionWidget = m_selectionWidget.get();
	FLAT_ASSERT(selectionWidget != nullptr);
	if (!selectionWidget->getParent().expired())
	{
		return selectionWidget->getSize().x > 1 && selectionWidget->getSize().y > 1;
	}
	return false;
}

void BaseMapState::updateMouseOverEntity(Game& game)
{
	m_mouseOverTile = nullptr;

	if ((isSelecting() && !isSmallSelection()) || isMouseOverUi(game))
	{
		m_mouseOverEntity = nullptr;
		return;
	}

	const flat::Vector2& mousePosition = game.input->mouse->getPosition();
	const flat::Vector2 viewMousePosition = m_gameView.getRelativePosition(mousePosition);

	entity::Entity* previousMouseOverEntity = m_mouseOverEntity.getEntity();
	entity::Entity* newMouseOverEntity = nullptr;

	map::MapObject* mouseOverObject = const_cast<map::MapObject*>(m_displayManager.getObjectAtPosition(viewMousePosition));
	if (mouseOverObject != nullptr)
	{
		if (mouseOverObject->isEntity())
		{
			entity::Entity* entity = static_cast<entity::Entity*>(mouseOverObject);
			newMouseOverEntity = entity;
		}
		else if (mouseOverObject->isTile())
		{
			m_mouseOverTile = static_cast<const map::Tile*>(mouseOverObject);
		}
	}

	if (previousMouseOverEntity != newMouseOverEntity)
	{
		if (previousMouseOverEntity != nullptr)
		{
			clearMouseOverColor(previousMouseOverEntity);
		}

		if (newMouseOverEntity != nullptr)
		{
			setMouseOverColor(newMouseOverEntity);
		}

		m_mouseOverEntity = newMouseOverEntity;
	}
}

void BaseMapState::clearMouseOverEntity()
{
	entity::Entity* mouseOverEntity = m_mouseOverEntity.getEntity();
	if (mouseOverEntity != nullptr)
	{
		clearMouseOverColor(mouseOverEntity);
		m_mouseOverEntity = nullptr;
	}
}

void BaseMapState::setMouseOverColor(entity::Entity* entity) const
{
	entity->getSprite().setColor(flat::video::Color::GREEN);
}

void BaseMapState::clearMouseOverColor(entity::Entity* entity) const
{
	flat::video::Color color = flat::video::Color::WHITE;
	if (entity->isSelected())
	{
		color = flat::video::Color::RED;
	}
	entity->getSprite().setColor(color);
}

bool BaseMapState::updateSelectionWidget(Game& game)
{
	if (isMouseOverUi(game) && !isSelecting())
	{
		return false;
	}

	const auto& mouse = game.input->mouse;
	const auto& keyboard = game.input->keyboard;
	const flat::Vector2& mousePosition = mouse->getPosition();

	if (mouse->isJustReleased(M(LEFT)))
	{
		flat::sharp::ui::Widget* selectionWidget = m_selectionWidget.get();
		if (!selectionWidget->getParent().expired())
		{
			const flat::Vector2& bottomLeft = selectionWidget->getPosition();
			flat::Vector2 topRight = bottomLeft + selectionWidget->getSize();

			const bool shiftPressed = keyboard->isPressed(K(LSHIFT));
			if (!isSmallSelection())
			{
				updateSelectedEntities(game, bottomLeft, topRight, shiftPressed);
			}

			selectionWidget->removeFromParent();

			return !m_selectedEntities.empty();
		}
	}
	else if (mouse->isJustPressed(M(LEFT)))
	{
		const bool shiftPressed = keyboard->isPressed(K(LSHIFT));
		selectClickedEntity(game, mousePosition, shiftPressed);

		m_mouseDownPosition = mousePosition;
		flat::sharp::ui::RootWidget* root = game.ui->root.get();
		root->addChild(m_selectionWidget);
	}

	if (mouse->isPressed(M(LEFT)))
	{
		// update selection bounds
		flat::Vector2 bottomLeft;
		bottomLeft.x = std::min(m_mouseDownPosition.x, mousePosition.x);
		bottomLeft.y = std::min(m_mouseDownPosition.y, mousePosition.y);
		flat::Vector2 topRight;
		topRight.x = std::max(m_mouseDownPosition.x, mousePosition.x);
		topRight.y = std::max(m_mouseDownPosition.y, mousePosition.y);
		flat::Vector2 size = topRight - bottomLeft;

		flat::sharp::ui::Widget* selectionWidget = m_selectionWidget.get();
		selectionWidget->setPosition(bottomLeft);
		selectionWidget->setSize(size);
		selectionWidget->setDirty();
		
		return true;
	}

	return false;
}

void BaseMapState::selectClickedEntity(Game& game, const flat::Vector2& mousePosition, bool addToSelection)
{
	if (!addToSelection)
	{
		clearSelection();
	}

	entity::Entity* mouseOverEntity = m_mouseOverEntity.getEntity();
	if (mouseOverEntity != nullptr)
	{
		clickEntity(mouseOverEntity);
		addToSelectedEntities(game, mouseOverEntity);
	}
}

void BaseMapState::selectEntitiesOfTypeInScreen(Game& game, const flat::Vector2& mousePosition, bool addToSelection)
{
	if (!addToSelection)
	{
		clearSelection();
	}

	entity::Entity* mouseOverEntity = m_mouseOverEntity.getEntity();
	if (mouseOverEntity != nullptr)
	{
		if (mouseOverEntity->canBeSelected() || forceEntitySelection(game))
		{
			const std::shared_ptr<const entity::EntityTemplate>& entityTemplate = mouseOverEntity->getEntityTemplate();

			flat::AABB2 screenAABB;
			m_gameView.getScreenAABB(screenAABB);

			std::vector<const map::MapObject*> entitiesInScreen;
			m_displayManager.getEntitiesInAABB(screenAABB, entitiesInScreen);

			for (const map::MapObject* mapObject : entitiesInScreen)
			{
				// TODO: fix these casts
				entity::Entity* entity = const_cast<entity::Entity*>(static_cast<const entity::Entity*>(mapObject));
				if (entity->getEntityTemplate() == entityTemplate)
				{
					addToSelectedEntities(game, entity);
				}
			}
		}
		else
		{
			// if the entity is not selectable, still trigger a click on double click
			clickEntity(mouseOverEntity);
		}
	}
}

void BaseMapState::updateSelectedEntities(Game& game, const flat::Vector2& bottomLeft, const flat::Vector2& topRight, bool addToSelection)
{
	if (!addToSelection)
	{
		clearSelection();
	}

	flat::AABB2 selectionAABB;
	selectionAABB.min = m_gameView.getRelativePosition(bottomLeft);
	selectionAABB.max = m_gameView.getRelativePosition(topRight);
	std::swap(selectionAABB.min.y, selectionAABB.max.y);

	std::vector<const map::MapObject*> entitiesInSelectionWidget;
	m_displayManager.getEntitiesInAABB(selectionAABB, entitiesInSelectionWidget);

	for (const map::MapObject* mapObject : entitiesInSelectionWidget)
	{
		// TODO: fix these casts
		entity::Entity* entity = const_cast<entity::Entity*>(static_cast<const entity::Entity*>(mapObject));
		if (!entity->canBeSelected() && !forceEntitySelection(game))
		{
			continue;
		}

		// check that the sprite origin actually is in the AABB
		if (selectionAABB.isInside(entity->getSprite().getPosition()))
		{
			addToSelectedEntities(game, entity);
		}
	}
}

void BaseMapState::clearSelection()
{
	for (entity::Entity* entity : m_selectedEntities)
	{
		entity->setSelected(false);
	}
	m_selectedEntities.clear();
}

void BaseMapState::addToSelectedEntities(Game& game, entity::Entity* entity)
{
	if ((entity->canBeSelected() || forceEntitySelection(game)) && !entity->isSelected())
	{
		entity->setSelected(true);
		FLAT_ASSERT(std::find(m_selectedEntities.begin(), m_selectedEntities.end(), entity) == m_selectedEntities.end());
		m_selectedEntities.push_back(entity);
	}
}

void BaseMapState::removeFromSelectedEntities(entity::Entity* entity)
{
	if (entity->isSelected())
	{
		// remove from selected entities
		std::vector<entity::Entity*>::iterator it = std::find(m_selectedEntities.begin(), m_selectedEntities.end(), entity);
		FLAT_ASSERT(it != m_selectedEntities.end());
		m_selectedEntities.erase(it);
	}
}

bool BaseMapState::isSmallSelection() const
{
	flat::sharp::ui::Widget* selectionWidget = m_selectionWidget.get();
	if (!selectionWidget->getParent().expired())
	{
		const flat::Vector2& selectionSize = selectionWidget->getSize();
		const float smallSize = 3.f;
		return selectionSize.x < smallSize && selectionSize.y < smallSize;
	}
	return false;
}

void BaseMapState::clickEntity(entity::Entity* entity) const
{
	entity::component::selection::SelectionComponent* selectionComponent = entity->getComponent<entity::component::selection::SelectionComponent>();
	if (selectionComponent != nullptr)
	{
		selectionComponent->click();
	}
}

bool BaseMapState::forceEntitySelection(Game& game) const
{
#ifdef FLAT_DEBUG
	auto& keyboard = game.input->keyboard;
	return keyboard->isPressed(K(LALT));
#else
	return false;
#endif
}

void BaseMapState::handleGameActionInputs(Game& game)
{
	const auto& mouse = game.input->mouse;
	const auto& keyboard = game.input->keyboard;

	if (isMouseOverUi(game) && !isSelecting())
	{
		clearMouseOverEntity();
	}
	else
	{
		updateMouseOverEntity(game);
		if (mouse->isJustDoubleClicked(M(LEFT)))
		{
			const bool shiftPressed = keyboard->isPressed(K(LSHIFT));
			selectEntitiesOfTypeInScreen(game, mouse->getPosition(), shiftPressed);
		}
		else if (m_ghostTemplate.get() != nullptr)
		{
			if (mouse->isJustPressed(M(LEFT)))
			{
				bool cursorOnTile;
				flat::Vector2 position2d = getCursorMapPosition(game, cursorOnTile);
				if (cursorOnTile)
				{
					const map::Tile* tile = getMap().getTileIfWalkable(position2d.x, position2d.y);
					if (tile != nullptr && canPlaceGhostEntity(tile))
					{
						if (onGhostEntityPlaced())
						{
							flat::Vector3 position(position2d.x, position2d.y, tile->getZ());
							spawnEntityAtPosition(game, m_ghostTemplate, position);
						}
						if (!keyboard->isPressed(K(LSHIFT)))
						{
							clearGhostTemplate();
						}
					}
				}
			}
			else if (mouse->isJustPressed(M(RIGHT)))
			{
				clearGhostTemplate();
			}
		}
		else
		{
			updateSelectionWidget(game);
		}

		if (mouse->isJustPressed(M(RIGHT)))
		{
			bool clickedOnTile;
			flat::Vector2 clickedTilePosition = getCursorMapPosition(game, clickedOnTile);
			if (clickedOnTile)
			{
				map::Tile* clickedTile = getMap().getTileIfWalkable(clickedTilePosition.x, clickedTilePosition.y);
				if (clickedTile)
				{
					if (!keyboard->isPressed(K(LSHIFT)))
					{
						for (entity::Entity* entity : m_selectedEntities)
						{
							if (entity->acceptsMoveOrders())
							{
								entity->clearPath();
							}
						}
					}

					for (entity::Entity* entity : m_selectedEntities)
					{
						if (entity->acceptsMoveOrders())
						{
							entity->addPointOnPath(clickedTilePosition);
						}
					}
				}
			}
		}
	}

	if (keyboard->isJustPressed(K(F)))
	{
		moveToFormation(game);
	}
}

void BaseMapState::moveToFormation(Game& game)
{
	// 1. compute centroid
	flat::Vector2 centroid(0.f, 0.f);
	for (entity::Entity* entity : m_selectedEntities)
	{
		centroid += flat::Vector2(entity->getPosition());
	}
	centroid /= m_selectedEntities.size();

	// 2. sort by farthest to closest
	std::vector<entity::Entity*> formationEntities = m_selectedEntities;
	std::sort(formationEntities.begin(), formationEntities.end(),
		[centroid](entity::Entity* a, entity::Entity* b)
	{
		//const float da = flat::length2(flat::Vector2(a->getPosition()) - centroid);
		//const float db = flat::length2(flat::Vector2(b->getPosition()) - centroid);
		const float da = std::abs(a->getPosition().x - centroid.x) + std::abs(a->getPosition().y - centroid.y);
		const float db = std::abs(b->getPosition().x - centroid.x) + std::abs(b->getPosition().y - centroid.y);
		return da < db;
	}
	);

	// 3. compute formation positions
	std::vector<flat::Vector2> formationPositions;
	formationPositions.reserve(formationEntities.size());
	const int squareSize = static_cast<int>(std::floor(std::sqrt(formationEntities.size())));
	const float distanceBetweenEntities = 0.7f;
	int row = 0;
	int col = 0;
	for (size_t i = 0; i < formationEntities.size(); ++i)
	{
		flat::Vector2 formationPosition;
		formationPosition.x = centroid.x + (static_cast<float>(col) - static_cast<float>(squareSize - 1) / 2.f) * distanceBetweenEntities;
		formationPosition.y = centroid.y + (static_cast<float>(row) - static_cast<float>(squareSize - 1) / 2.f) * distanceBetweenEntities;
		++col;
		if (col >= squareSize)
		{
			col = 0;
			++row;
		}
		formationPositions.push_back(formationPosition);
	}
	FLAT_ASSERT(formationPositions.size() == formationEntities.size());

	// 4. find closest formation point for each entity and remove point from available positions
	for (entity::Entity* entity : formationEntities)
	{
		// find closest solution
		std::vector<flat::Vector2>::iterator closestPositionIt = formationPositions.end();
		float closestPositionDistance2 = std::numeric_limits<float>::max();
		for (std::vector<flat::Vector2>::iterator it = formationPositions.begin(); it != formationPositions.end(); ++it)
		{
			const float distanceToPosition2 = flat::length2(*it - flat::Vector2(entity->getPosition()));
			if (distanceToPosition2 < closestPositionDistance2)
			{
				closestPositionDistance2 = distanceToPosition2;
				closestPositionIt = it;
			}
		}
		FLAT_ASSERT(closestPositionIt != formationPositions.end());
		entity->clearPath();
		entity->addPointOnPath(*closestPositionIt);
		formationPositions.erase(closestPositionIt);
	}
	FLAT_ASSERT(formationPositions.empty());
}

void BaseMapState::updateEntities()
{
#ifdef FLAT_DEBUG
	if (!isGamePaused())
	{
#endif

	despawnEntities();
	const flat::time::Clock& clock = getClock();
	m_entityUpdater.updateAllEntities(clock.getTime(), clock.getDT());

#ifdef FLAT_DEBUG
	}
#endif
}

#ifdef FLAT_DEBUG
void BaseMapState::copyStateBeforeReload(const BaseMapState& other)
{
	setModPath(other.m_mod.getPath());
	m_gameView = other.m_gameView;
	m_cameraCenter2d = other.m_cameraCenter2d;
	m_cameraZoom = other.m_cameraZoom;

	m_isReloading = true;
}
#endif

} // states
} // game


