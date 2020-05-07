#include "basemapstate.h"
#include "selectmapstate.h"
#include "mapeditor/lua/editor.h"

#include "game.h"

#include "map/map.h"
#include "map/tile.h"
#include "map/tiletemplate.h"
#include "map/lua/map.h"
#include "map/lua/zone.h"
#include "map/proptemplate.h"
#include "map/fog/nofog.h"
#include "map/brush/lua/brush.h"

#include "entity/entityhelper.h"
#include "entity/entitytemplate.h"
#include "entity/component/components/attack/attackcomponent.h"
#include "entity/component/components/behavior/behaviorcomponent.h"
#include "entity/component/components/collision/collisioncomponent.h"
#include "entity/component/components/interaction/interactioncomponent.h"
#include "entity/component/components/selection/selectioncomponent.h"
#include "entity/component/components/fogvision/fogvisioncomponent.h"
#include "entity/component/lua/componentregistry.h"
#include "entity/faction/lua/faction.h"

namespace game
{
namespace states
{

BaseMapState::BaseMapState() :
	m_entityPool(m_componentRegistry),
	m_entityUpdater(m_componentRegistry),
	m_cameraLocked(false)
#ifdef FLAT_DEBUG
	, m_isReloading(false)
#endif
{
	m_selectionChangedSlotProxy.init(
		&selectionChanged,
		[](lua_State* L) {}
	);
}

void BaseMapState::enter(Game& game)
{
	Super::enter(game);

	// time
	m_gameClock = game.time->newClock();
	m_gameTimerContainer = game.lua->newTimerContainer(m_gameClock);
#ifdef FLAT_DEBUG
	m_gamePaused = false;
	m_pauseNextFrame = false;
#endif
	// ui
	buildUi(game);
	game.input->pushContext(m_gameInputContext);

	// rendering settings
	m_entityRender.program.load("data/shaders/sprite/entityspritebatch.frag", "data/shaders/sprite/entityspritebatch.vert");

	m_entityRender.settings.textureUniform              = m_entityRender.program.getUniform<flat::video::Texture>("objectTexture");
	m_entityRender.settings.viewProjectionMatrixUniform = m_entityRender.program.getUniform<flat::Matrix4>("vpMatrix");

	m_entityRender.settings.positionAttribute           = m_entityRender.program.getAttribute("position");
	m_entityRender.settings.uvAttribute                 = m_entityRender.program.getAttribute("uv");
	m_entityRender.settings.colorAttribute              = m_entityRender.program.getAttribute("color");
	//m_entityRender.settings.normalAttribute             = m_entityRender.program.getAttribute("normal");
	m_entityRender.settings.depthAttribute              = m_entityRender.program.getAttribute("depth");

	entity::Entity::setEntityProgramSettings(m_entityRender);

	m_terrainRender.program.load("data/shaders/sprite/terrainspritebatch.frag", "data/shaders/sprite/terrainspritebatch.vert");

	m_terrainRender.settings.textureUniform              = m_terrainRender.program.getUniform<flat::video::Texture>("objectTexture");
	m_terrainRender.settings.viewProjectionMatrixUniform = m_terrainRender.program.getUniform<flat::Matrix4>("vpMatrix");

	m_terrainRender.settings.positionAttribute           = m_terrainRender.program.getAttribute("position");
	m_terrainRender.settings.uvAttribute                 = m_terrainRender.program.getAttribute("uv");
	m_terrainRender.settings.colorAttribute              = m_terrainRender.program.getAttribute("color");
	m_terrainRender.settings.normalAttribute             = m_terrainRender.program.getAttribute("normal");
	m_terrainRender.settings.depthAttribute              = m_terrainRender.program.getAttribute("depth");

	map::Tile::setTileProgramSettings(m_terrainRender);

	initRender(game);

	game.lua->doFile("flat-engine/lua/debug/debug.lua");

	// reset the game view *before* loading the map so the ui components can be initialized properly
#ifdef FLAT_DEBUG
	if (!m_isReloading)
	{
#endif
		m_gameView.setWindow(game.video->window);
		m_gameView.updateProjection();
		m_cameraZoom = 1.f;
#ifdef FLAT_DEBUG
	}
#endif
	m_map.setDisplayManager(&m_displayManager);
	loadMap(game);

	// reset the camera *after* loading the map so it's centered on the map
#ifdef FLAT_DEBUG
	if (!m_isReloading)
	{
#endif
		int minX, maxX, minY, maxY;
		m_map.getBounds(minX, maxX, minY, maxY);
		flat::Vector3 cameraCenter((maxX + minX) / 2.f, (maxY + minY) / 2.f, 0.f);
		setCameraCenter(cameraCenter);
#ifdef FLAT_DEBUG
	}
#endif

	// load debug display resources *after* the map is loaded!
	FLAT_DEBUG_ONLY(m_debugDisplay.loadResources(game);)

	resetViews(game);

	m_mouseOverEntity = nullptr;
	m_mouseOverTileIndex = map::TileIndex::INVALID_TILE;
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
		game.message->confirm("Are you sure you want to quit?", [&game](bool result)
		{
			if (result)
			{
				std::unique_ptr<SelectMapState> selectMapState = std::make_unique<SelectMapState>();
				game.getStateMachine().setNextState(std::move(selectMapState));
			}
		});
	}

	update(game);

	clearScreen(game);
	draw(game);
}

void BaseMapState::exit(Game& game)
{
	std::vector<entity::Entity*> entities = m_entityUpdater.getEntities();
	for (entity::Entity* entity : entities)
	{
		despawnEntity(entity);
	}
	clearGhostTemplate();

	if (!isMouseOverUi(game))
	{
		game.input->popContext(m_gameInputContext);
	}

	Super::exit(game);
}

bool BaseMapState::loadMap(Game& game)
{
	return m_map.load(game);
}

bool BaseMapState::saveMap(Game& game) const
{
	return m_map.save(game, m_entityUpdater.getEntities());
}

map::Map& BaseMapState::getMap()
{
	return m_map;
}

const map::Map& BaseMapState::getMap() const
{
	return m_map;
}

flat::Vector2 BaseMapState::getCursorMapPosition(game::Game& game, bool& isOnTile) const
{
	const flat::Vector2& cursorPosition = m_gameInputContext->getMouseInputContext().getPosition();
	flat::Vector2 gameViewPosition = m_gameView.getRelativePosition(cursorPosition);

	const flat::Vector2& xAxis = m_map.getXAxis();
	const flat::Vector2& yAxis = m_map.getYAxis();

	auto gameViewToMap = [&xAxis, &yAxis](const flat::Vector2& screenPosition)
	{
		return flat::Vector2(
			(screenPosition.x * yAxis.y - screenPosition.y * yAxis.x) / (xAxis.x * yAxis.y - xAxis.y * yAxis.x),
			(screenPosition.y * xAxis.x - screenPosition.x * xAxis.y) / (yAxis.y * xAxis.x - yAxis.x * xAxis.y)
		);
	};

	flat::Vector2 mapPosition = gameViewToMap(gameViewPosition);
	isOnTile = false;

	if (m_mouseOverTileIndex != map::TileIndex::INVALID_TILE)
	{
		const flat::Vector2& spritePosition = m_map.getTileSprite(m_mouseOverTileIndex).getPosition();
		flat::Vector2 delta = gameViewToMap(gameViewPosition - spritePosition);

		const flat::Vector2i& xy = m_map.getTileXY(m_mouseOverTileIndex);
		flat::Vector2 tileCenter = flat::Vector2(xy.x, xy.y);
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

			map::TileIndex adjacentTileIndex = m_map.getTileIndex(adjacentTilePosition.x, adjacentTilePosition.y);
			if (adjacentTileIndex != map::TileIndex::INVALID_TILE)
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
	map::TileIndex tileIndex = m_map.getFog().getTileIndex(position2d.x, position2d.y);
	if (map::isValidTile(tileIndex))
	{
		const float tileZ = m_map.getTileZ(tileIndex);
		position3d.z = tileZ;
		color = flat::video::Color::BLUE;

		{
			const flat::Vector2i& xy = m_map.getTileXY(tileIndex);
			flat::Vector3 position3d(xy.x, xy.y, tileZ);
			m_debugDisplay.add3dLine(position3d + flat::Vector3(-0.5f, -0.5f, 0.f), position3d + flat::Vector3(0.5f, -0.5f, 0.f));
			m_debugDisplay.add3dLine(position3d + flat::Vector3(0.5f, -0.5f, 0.f), position3d + flat::Vector3(0.5f, 0.5f, 0.f));
			m_debugDisplay.add3dLine(position3d + flat::Vector3(0.5f, 0.5f, 0.f), position3d + flat::Vector3(-0.5f, 0.5f, 0.f));
			m_debugDisplay.add3dLine(position3d + flat::Vector3(-0.5f, 0.5f, 0.f), position3d + flat::Vector3(-0.5f, -0.5f, 0.f));
		}

		const map::Tile& tile = m_map.getTileFromIndex(tileIndex);
		const flat::AABB2& tileAABB = tile.getAABB();
		m_debugDisplay.add2dAABB(tileAABB, flat::video::Color::GREEN);

		const flat::AABB2& cellAABB = m_displayManager.getTileCellAABB(tileIndex);
		m_debugDisplay.add2dAABB(cellAABB, flat::video::Color::RED);

	}
	m_debugDisplay.add3dCircle(position3d, 0.1f, color, 1.f);

	std::string str = std::to_string(position3d.x) + "," + std::to_string(position3d.y) + "\n" + std::to_string(tileIndex);
	m_debugDisplay.add3dText(position3d + flat::Vector3(0.f, 0.f, -1.f), str);
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
	std::string entityTemplatePath = entityTemplateName;
	if (entityTemplatePath.find('/') == std::string::npos
		&& entityTemplatePath.find('\\') == std::string::npos)
	{
		const flat::tool::Asset* asset = game.assetRepository->findAssetFromName("entity", entityTemplateName);
		if (asset == nullptr)
		{
			// will return an empty entity template
			return m_entityTemplateManager.getResource(entityTemplatePath, game, m_componentRegistry);
		}
		entityTemplatePath = asset->getPath().string();
	}
	return m_entityTemplateManager.getResource(entityTemplatePath, game, m_componentRegistry);
}

std::shared_ptr<const map::TileTemplate> BaseMapState::getTileTemplate(game::Game& game, const std::string& tileTemplateName) const
{
	return m_tileTemplateManager.getResource(tileTemplateName, game);
}

std::shared_ptr<const map::PropTemplate> BaseMapState::getPropTemplate(game::Game& game, const std::string& propTemplateName) const
{
	const std::string propTemplatePath = game.mod.getPropTemplatePath(propTemplateName);
	return m_propTemplateManager.getResource(propTemplatePath, game);
}

entity::Entity* BaseMapState::spawnEntityAtPosition(
	Game& game,
	const std::shared_ptr<const entity::EntityTemplate>& entityTemplate,
	const flat::Vector3& position,
	float heading,
	float elevation,
	entity::Entity* instigator,
	entity::component::ComponentFlags componentFlags,
	entity::component::ComponentFlags enabledComponentFlags
)
{
	FLAT_PROFILE("Spawn entity");

	entity::Entity* entity = createEntity(game, entityTemplate, componentFlags);
	entity->setInstigator(instigator);

	{
		FLAT_PROFILE("Disable useless components");
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
	}

	{
		FLAT_PROFILE("Set initial position");
		entity->setPosition(position);
		entity->setHeading(heading);
		entity->setElevation(elevation);
	}

	if (!addEntityToMap(entity))
	{
		destroyEntity(entity);
		game.notify->warn(std::string("Cannot spawn entity ") + entityTemplate->getPath());
		return nullptr;
	}

#ifdef FLAT_DEBUG
	entity->checkSpriteAABB();
#endif

	return entity;
}

void BaseMapState::despawnEntity(entity::Entity* entity)
{
	FLAT_PROFILE("Despawn entity");

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
}

void BaseMapState::clearGhostTemplate()
{
	m_ghostTemplate.reset();
}

std::vector<flat::Vector2> BaseMapState::getGhostEntityPositions(const flat::Vector2& cursorPosition, map::TileIndex tileIndex) const
{
	FLAT_ASSERT(map::isValidTile(tileIndex));
	return { cursorPosition };
}

bool BaseMapState::onGhostEntityPlaced(map::TileIndex tileIndex, bool& continueAction)
{
	FLAT_ASSERT(map::isValidTile(tileIndex));
	return true;
}

entity::Entity* BaseMapState::createEntity(Game& game, const std::shared_ptr<const entity::EntityTemplate>& entityTemplate, entity::component::ComponentFlags componentFlags)
{
	FLAT_PROFILE("Create entity");

	entity::component::ComponentFlags componentsFilter = getComponentsFilter() & componentFlags;
	entity::Entity* entity = m_entityPool.createEntity(entityTemplate, m_componentRegistry, componentsFilter);
	return entity;
}

void BaseMapState::destroyEntity(entity::Entity* entity)
{
	FLAT_ASSERT(entity->getMap() == nullptr);
	m_entityPool.destroyEntity(entity);
}

bool BaseMapState::addEntityToMap(entity::Entity* entity)
{
	FLAT_PROFILE("Add entity to map");
	m_entityUpdater.registerEntity(entity);
	if (!entity->addToMap(&m_map))
	{
		m_entityUpdater.unregisterEntity(entity);
		return false;
	}
	flat::time::Clock& clock = getGameClock();
	m_displayManager.addEntity(entity);
	m_entityUpdater.updateSingleEntity(entity, clock.getTime(), clock.getDT());
	return true;
}

void BaseMapState::removeEntityFromMap(entity::Entity* entity)
{
	FLAT_ASSERT(entity->getMap() != nullptr);
	m_entityUpdater.unregisterEntity(entity);
	entity->removeFromMap();
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

	flat::time::Clock& clock = getGameClock();
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
	Super::update(game);

	//debugCursorPosition(game);
}

std::vector<entity::Entity*> BaseMapState::addGhostEntities(game::Game& game)
{
	std::vector<entity::Entity*> entities;
	if (!isMouseOverUi(game) && !isSelecting() && !m_mouseOverEntity.isValid() && m_ghostTemplate != nullptr)
	{
		bool isOnTile;
		const flat::Vector2 cursorPosition = getCursorMapPosition(game, isOnTile);
		if (isOnTile)
		{
			const map::Navigability navigabilityMask = entity::EntityHelper::getNavigabilityMask(m_ghostTemplate.get());
			const map::TileIndex tileIndex = m_map.getTileIndexIfNavigable(cursorPosition.x, cursorPosition.y, navigabilityMask);
			if (map::isValidTile(tileIndex))
			{
				using namespace entity::component;
				ComponentFlags componentFlags = AllComponents;
				componentFlags &= ~attack::AttackComponent::getFlag();
				componentFlags &= ~behavior::BehaviorComponent::getFlag();
				componentFlags &= ~collision::CollisionComponent::getFlag();
				componentFlags &= ~fogvision::FogVisionComponent::getFlag();
				const std::vector<flat::Vector2> ghostEntityPositions = getGhostEntityPositions(cursorPosition, tileIndex);
				entities.reserve(ghostEntityPositions.size());
				for (const flat::Vector2& ghostEntityPosition : ghostEntityPositions)
				{
					flat::Vector3 ghostPosition(ghostEntityPosition, m_map.getTileZ(m_map.getTileIndex(ghostEntityPosition)));

					entity::Entity* ghost = spawnEntityAtPosition(game, m_ghostTemplate, ghostPosition, 0.f, 0.f, nullptr, componentFlags);
					if (ghost == nullptr)
					{
						continue;
					}

					entities.push_back(ghost);
					flat::render::BaseSprite& sprite = entities.back()->getSprite();
					flat::video::Color color = flat::video::Color::WHITE;
					color.a = 0.6f;
					sprite.setColor(color);
#ifdef FLAT_DEBUG
					entities.back()->debugDraw(m_debugDisplay);
#endif
				}
			}
		}
	}
	return entities;
}

void BaseMapState::updateGameView(game::Game& game)
{
	if (m_cameraLocked > 0)
	{
		return;
	}

	const auto& keyboard = m_gameInputContext->getKeyboardInputContext();
	const auto& mouse = m_gameInputContext->getMouseInputContext();
	
	const flat::Vector2& xAxis = m_map.getXAxis();
	flat::Vector2 speed(-xAxis.x, xAxis.y);
	speed /= m_cameraZoom;
	
	flat::Vector2 move;
	
	// keyboard wins over mouse
	const bool leftPressed = keyboard.isPressed(K(LEFT));
	const bool rightPressed = keyboard.isPressed(K(RIGHT));
	const bool upPressed = keyboard.isPressed(K(UP));
	const bool downPressed = keyboard.isPressed(K(DOWN));
	
	if (leftPressed && !rightPressed)
		move.x = -speed.x;
	else if (rightPressed && !leftPressed)
		move.x = speed.x;
		
	if (upPressed && !downPressed)
		move.y = -speed.y;
	else if (downPressed && !upPressed)
		move.y = speed.y;
	
	const float cameraSpeed = 40.f;
	flat::time::Clock& uiClock = *game.time->defaultClock;
	m_cameraCenter2d += move * uiClock.getDT() * cameraSpeed;
	updateCameraView();

	if (mouse.wheelJustMoved() && !keyboard.isPressed(K(SPACE)))
	{
		const float zoom = m_cameraZoom * static_cast<float>(std::pow(2, mouse.getWheelMove().y));
		setCameraZoom(zoom);
	}
	
	if (game.input->window->isResized())
	{
		m_gameView.updateProjection();
	}
}

void BaseMapState::setCameraCenter(const flat::Vector2& cameraCenter)
{
	m_cameraCenter2d = cameraCenter;
	updateCameraView();
}

void BaseMapState::setCameraCenter(const flat::Vector3& cameraCenter)
{
	setCameraCenter(convertToCameraPosition(cameraCenter));
}

void BaseMapState::setCameraZoom(float cameraZoom)
{
	m_cameraZoom = cameraZoom;
	updateCameraView();
}

void BaseMapState::unlockCamera()
{
	m_cameraLocked--;
	if (m_cameraLocked < 0)
	{
		m_cameraLocked = 0;
	}
}

flat::Vector2 BaseMapState::convertToCameraPosition(const flat::Vector3& position) const
{
	return flat::Vector2(m_map.getTransform() * position);
}

void BaseMapState::updateCameraView()
{
	m_gameView.reset();
	m_gameView.flipY();
	m_gameView.zoom(m_cameraZoom);
	m_gameView.move(m_cameraCenter2d);
}

void BaseMapState::initLua(Game& game)
{
	Super::initLua(game);

	lua_State* L = game.lua->state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		entity::component::lua::open(L, m_componentRegistry);
		entity::faction::lua::open(L, game.mod.getFactionsConfigPath());
		map::lua::map::open(L);
		editor::lua::open(L);

		flat::lua::doFile(L, "data/common/init.lua");
	}
}

void BaseMapState::draw(game::Game& game)
{
	FLAT_PROFILE("Draw");

	// map
	{
		FLAT_PROFILE("Draw map");

		std::vector<entity::Entity*> ghosts = addGhostEntities(game);
		m_displayManager.sortAndDraw(game, m_map.getFog(), m_gameView);
		for(entity::Entity* ghost: ghosts)
		{
			despawnEntity(ghost);
		}
	}

#ifdef FLAT_DEBUG
	{
		FLAT_PROFILE("Draw debug");

		m_map.debugDraw(m_debugDisplay);
		m_entityUpdater.debugDraw(m_debugDisplay);
		m_debugDisplay.drawElements(game, m_gameView);
	}
#endif

	Super::draw(game);
}

void BaseMapState::buildUi(game::Game& game)
{
	m_gameInputContext = std::make_shared<flat::input::context::InputContext>(game);

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
	m_mouseOverTileIndex = map::TileIndex::INVALID_TILE;

	if ((isSelecting() && !isSmallSelection()) || isMouseOverUi(game))
	{
		m_mouseOverEntity = nullptr;
		return;
	}

	const flat::Vector2& mousePosition = m_gameInputContext->getMouseInputContext().getPosition();
	const flat::Vector2 viewMousePosition = m_gameView.getRelativePosition(mousePosition);

	entity::Entity* previousMouseOverEntity = m_mouseOverEntity.getEntity();
	entity::Entity* newMouseOverEntity = nullptr;

	map::MapObject* mouseOverObject = const_cast<map::MapObject*>(m_displayManager.getObjectAtPosition(m_map.getFog(), viewMousePosition));
	if (mouseOverObject != nullptr)
	{
		if (mouseOverObject->isEntity())
		{
			entity::Entity* entity = static_cast<entity::Entity*>(mouseOverObject);
			newMouseOverEntity = entity;
		}

		if (mouseOverObject->isTile())
		{
			m_mouseOverTileIndex = m_map.getFog().getTileIndex(static_cast<const map::Tile*>(mouseOverObject));
		}
		else
		{
			m_mouseOverTileIndex = m_displayManager.getTileIndexAtPosition(m_map.getFog(), viewMousePosition);
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
	}

	m_mouseOverEntity = newMouseOverEntity;
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

	flat::sharp::ui::Widget* selectionWidget = m_selectionWidget.get();

	if (game.input->mouse->isJustReleased(M(LEFT)))
	{
		if (!selectionWidget->getParent().expired())
		{
			const flat::Vector2& bottomLeft = selectionWidget->getPosition();
			flat::Vector2 topRight = bottomLeft + selectionWidget->getSize();

			const bool shiftPressed = game.input->keyboard->isPressed(K(LSHIFT));
			if (!isSmallSelection())
			{
				updateSelectedEntities(game, bottomLeft, topRight, shiftPressed);
			}

			selectionWidget->removeFromParent();

			return !m_selectedEntities.empty();
		}
	}
	else if (m_gameInputContext->getMouseInputContext().isJustPressed(M(LEFT)))
	{
		const bool shiftPressed = game.input->keyboard->isPressed(K(LSHIFT));
		const flat::Vector2& mousePosition = game.input->mouse->getPosition();
		selectClickedEntity(game, mousePosition, shiftPressed);

		m_mouseDownPosition = mousePosition;
		flat::sharp::ui::RootWidget* root = game.ui->root.get();
		root->addChild(m_selectionWidget);
	}

	for (entity::EntityHandle entityHandle : m_entitiesInSelection)
	{
		entity::Entity* entity = entityHandle.getEntity();
		if (entity != nullptr)
		{
			clearMouseOverColor(entity);
		}
	}
	m_entitiesInSelection.clear();

	if (!selectionWidget->getParent().expired())
	{
		const flat::Vector2& mousePosition = game.input->mouse->getPosition();

		// update selection bounds
		flat::Vector2 bottomLeft;
		bottomLeft.x = std::min(m_mouseDownPosition.x, mousePosition.x);
		bottomLeft.y = std::min(m_mouseDownPosition.y, mousePosition.y);
		flat::Vector2 topRight;
		topRight.x = std::max(m_mouseDownPosition.x, mousePosition.x);
		topRight.y = std::max(m_mouseDownPosition.y, mousePosition.y);
		flat::Vector2 size = topRight - bottomLeft;

		selectionWidget->setPosition(bottomLeft);
		selectionWidget->setSize(size);
		selectionWidget->setDirty();

		std::vector<entity::Entity*> entitiesInSelection;
		getEntitiesInSelection(bottomLeft, topRight, entitiesInSelection);
		for (entity::Entity* entity : entitiesInSelection)
		{
			if (!entity->isSelected())
			{
				m_entitiesInSelection.push_back(entity->getHandle());
				setMouseOverColor(entity);
			}
		}
		
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
		if (addToSelectedEntities(game, mouseOverEntity))
		{
			selectionChanged();
		}
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
			selectionChanged();
		}
		else
		{
			// if the entity is not selectable, still trigger a click on double click
			clickEntity(mouseOverEntity);
		}
	}
}

void BaseMapState::getEntitiesInSelection(const flat::Vector2& bottomLeft, const flat::Vector2& topRight, std::vector<entity::Entity*>& entities) const
{
	flat::AABB2 selectionAABB;
	selectionAABB.min = m_gameView.getRelativePosition(bottomLeft);
	selectionAABB.max = m_gameView.getRelativePosition(topRight);
	std::swap(selectionAABB.min.y, selectionAABB.max.y);

	std::vector<const map::MapObject*> entitiesInSelectionWidget;
	m_displayManager.getEntitiesInAABB(selectionAABB, entitiesInSelectionWidget);

	entities.clear();
	entities.reserve(entitiesInSelectionWidget.size());
	for (const map::MapObject* mapObject : entitiesInSelectionWidget)
	{
		FLAT_ASSERT(mapObject->isEntity());

		// check that the sprite origin actually is in the AABB
		if (selectionAABB.isInside(mapObject->getSprite().getPosition()))
		{
			// TODO: fix these casts
			entities.push_back(const_cast<entity::Entity*>(static_cast<const entity::Entity*>(mapObject)));
		}
	}
}

void BaseMapState::updateSelectedEntities(Game& game, const flat::Vector2& bottomLeft, const flat::Vector2& topRight, bool addToSelection)
{
	if (!addToSelection)
	{
		clearSelection();
	}

	std::vector<entity::Entity*> entitiesInSelectionWidget;
	getEntitiesInSelection(bottomLeft, topRight, entitiesInSelectionWidget);

	const int numSelectedEntities = static_cast<int>(m_selectedEntities.size());

	for (entity::Entity* entity : entitiesInSelectionWidget)
	{
		if (!entity->canBeSelected() && !forceEntitySelection(game))
		{
			continue;
		}
		addToSelectedEntities(game, entity);
	}

	if (static_cast<int>(m_selectedEntities.size()) > numSelectedEntities)
	{
		selectionChanged();
	}
}

void BaseMapState::clearSelection()
{
	if (!m_selectedEntities.empty())
	{
		for (entity::Entity* entity : m_selectedEntities)
		{
			entity->setSelected(false);
		}
		m_selectedEntities.clear();
		selectionChanged();
	}
}

bool BaseMapState::addToSelectedEntities(Game& game, entity::Entity* entity)
{
	if ((entity->canBeSelected() || forceEntitySelection(game)) && !entity->isSelected())
	{
		entity->setSelected(true);
		FLAT_ASSERT(std::find(m_selectedEntities.begin(), m_selectedEntities.end(), entity) == m_selectedEntities.end());
		m_selectedEntities.push_back(entity);
		return true;
	}
	return false;
}

void BaseMapState::removeFromSelectedEntities(entity::Entity* entity)
{
	if (entity->isSelected())
	{
		// remove from selected entities
		std::vector<entity::Entity*>::iterator it = std::find(m_selectedEntities.begin(), m_selectedEntities.end(), entity);
		FLAT_ASSERT(it != m_selectedEntities.end());
		m_selectedEntities.erase(it);
		selectionChanged();
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

int BaseMapState::addSelectionChangedCallback(lua_State* L, int index)
{
	return m_selectionChangedSlotProxy.addCallback(L, index);
}

void BaseMapState::removeSelectionChangedCallback(int index)
{
	m_selectionChangedSlotProxy.removeCallback(index);
}

void BaseMapState::handleGameActionInputs(Game& game)
{
	FLAT_PROFILE("Handle inputs");

	const auto& keyboard = m_gameInputContext->getKeyboardInputContext();
	const auto& mouse = m_gameInputContext->getMouseInputContext();

	if (isMouseOverUi(game) && !isSelecting())
	{
		clearMouseOverEntity();
	}
	else
	{
		updateMouseOverEntity(game);
		if (mouse.isJustDoubleClicked(M(LEFT)))
		{
			const bool shiftPressed = keyboard.isPressed(K(LSHIFT));
			selectEntitiesOfTypeInScreen(game, mouse.getPosition(), shiftPressed);
		}
		else if (m_ghostTemplate.get() != nullptr)
		{
			if (mouse.isJustPressed(M(LEFT)))
			{
				bool cursorOnTile;
				flat::Vector2 position2d = getCursorMapPosition(game, cursorOnTile);
				if (cursorOnTile)
				{
					map::Navigability navigabilityMask = entity::EntityHelper::getNavigabilityMask(m_ghostTemplate.get());
					map::TileIndex tileIndex = m_map.getTileIndexIfNavigable(position2d.x, position2d.y, navigabilityMask);
					if (tileIndex != map::TileIndex::INVALID_TILE)
					{
						bool continueAction = keyboard.isPressed(K(LSHIFT));
						const bool createEntity = onGhostEntityPlaced(tileIndex, continueAction);
						if (createEntity)
						{
							flat::Vector3 position(position2d.x, position2d.y, m_map.getTileZ(tileIndex));
							spawnEntityAtPosition(game, m_ghostTemplate, position);
						}
						if (!continueAction)
						{
							clearGhostTemplate();
						}
					}
				}
			}
			else if (mouse.isJustPressed(M(RIGHT)))
			{
				clearGhostTemplate();
			}
		}
		else
		{
			updateSelectionWidget(game);
		}

		if (mouse.isJustPressed(M(RIGHT)))
		{
			bool clickedOnTile;
			flat::Vector2 clickedTilePosition = getCursorMapPosition(game, clickedOnTile);

			if (clickedOnTile)
			{
				entity::Entity* interactionEntity = m_mouseOverEntity.getEntity();
				if (interactionEntity != nullptr)
				{
					clickedTilePosition = flat::Vector2(interactionEntity->getPosition());
				}

				const bool shiftPressed = keyboard.isPressed(K(LSHIFT));

				if (interactionEntity != nullptr)
				{
					// try interacting or else only move to the clicked entity
					entity::component::interaction::InteractionComponent* interactionComponent = interactionEntity->getComponent<entity::component::interaction::InteractionComponent>();
					if (interactionComponent != nullptr)
					{
						const char* behaviorStateName = interactionComponent->getBehaviorStateName().data();
						for (entity::Entity* entity : m_selectedEntities)
						{
							if (entity->acceptsPlayerMoveOrder(clickedTilePosition, interactionEntity))
							{
								if (!shiftPressed)
								{
									entity->cancelCurrentActions();
								}
								entity->moveTo(clickedTilePosition, interactionEntity, false);
							}
							if (entity->canInteract())
							{
								entity->setInteractionIfCompatible(behaviorStateName, interactionEntity);
							}
						}
					}
				}
				else
				{
					for (entity::Entity* entity : m_selectedEntities)
					{
						if (entity->acceptsPlayerMoveOrder(clickedTilePosition))
						{
							if (!shiftPressed)
							{
								entity->cancelCurrentActions();
							}
							entity->moveTo(clickedTilePosition, nullptr, false);
						}
					}
				}
			}
		}
	}

	if (keyboard.isJustPressed(K(F)))
	{
		moveToFormation(game);
	}
}

void BaseMapState::moveToFormation(Game& game)
{
	// 0. filter out entities with no movement component
	std::vector<entity::Entity*> formationEntities = m_selectedEntities;
	formationEntities.erase(std::remove_if(
		formationEntities.begin(),
		formationEntities.end(),
		[](const entity::Entity* a) { return a->getComponent<entity::component::movement::MovementComponent>() == nullptr; }
	), formationEntities.end());

	// 1. compute centroid
	flat::Vector2 centroid(0.f, 0.f);
	for (entity::Entity* entity : formationEntities)
	{
		centroid += flat::Vector2(entity->getPosition());
	}
	centroid /= formationEntities.size();

	// 2. sort by farthest to closest
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
		entity->cancelCurrentActions();
		entity->moveTo(*closestPositionIt);
		formationPositions.erase(closestPositionIt);
	}
	FLAT_ASSERT(formationPositions.empty());
}

void BaseMapState::updateEntities()
{
	FLAT_PROFILE("Update entities");

#ifdef FLAT_DEBUG
	if (!isGamePaused())
	{
#endif

	m_map.getFog().preUpdate();

	despawnEntities();
	const flat::time::Clock& clock = getGameClock();
	m_entityUpdater.updateAllEntities(clock.getTime(), clock.getDT());

	m_map.getFog().postUpdate();

#ifdef FLAT_DEBUG
	}
#endif
}

void BaseMapState::updateMap(Game& game)
{
	FLAT_PROFILE("Update map");

	const flat::time::Clock& clock = getGameClock();
	m_map.update(game, clock.getTime());
}

#ifdef FLAT_DEBUG
void BaseMapState::copyStateBeforeReload(const BaseMapState& other)
{
	m_gameView = other.m_gameView;
	m_cameraCenter2d = other.m_cameraCenter2d;
	m_cameraZoom = other.m_cameraZoom;

	m_isReloading = true;
}
#endif

} // states
} // game


