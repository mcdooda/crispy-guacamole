#include <flat.h>
#include "basemapstate.h"
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
#include "../entity/component/components/collision/collisioncomponent.h"
#include "../entity/faction/lua/faction.h"
#include "../entity/entitytemplate.h"
#include "../mod/lua/mod.h"

namespace game
{
namespace states
{

BaseMapState::BaseMapState() :
	m_entityPool(m_componentRegistry)
#ifdef FLAT_DEBUG
	, m_isReloading(false)
#endif
{

}

void BaseMapState::enter(Game& game)
{
	game.video->window->setTitle("Crispy guacamole");

	// init time first
	m_clock = game.time->newClock();
	m_uiClock = game.time->newClock();
	m_timerContainer.setClock(m_clock);

#ifdef FLAT_DEBUG
	if (m_isReloading)
	{
		game.lua->clearLoadedPackages();
	}
#endif
	
	// init lua then
	lua_State* L = game.lua->state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		timer::lua::open(L);
		entity::lua::open(L);
		entity::component::lua::open(L, m_componentRegistry);
		entity::faction::lua::open(L, m_mod.getFactionsConfigPath());
		mod::lua::open(L);
		map::lua::map::open(L);
		map::lua::zone::open(L);
		map::brush::lua::open(L);
		editor::lua::open(L);
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
	
	m_uiRender.program.load("data/shaders/ui/ui.frag", "data/shaders/ui/ui.vert");
	
	m_uiRender.settings.textureUniform              = m_uiRender.program.getUniform<flat::video::Texture>("objectTexture");
	m_uiRender.settings.textureGivenUniform         = m_uiRender.program.getUniform<bool>("textureGiven");
	m_uiRender.settings.colorUniform                = m_uiRender.program.getUniform<flat::video::Color>("color");
	m_uiRender.settings.secondaryColorUniform       = m_uiRender.program.getUniform<flat::video::Color>("secondaryColor");
	m_uiRender.settings.modelMatrixUniform          = m_uiRender.program.getUniform<flat::Matrix4>("modelMatrix");
	m_uiRender.settings.viewProjectionMatrixUniform = m_uiRender.program.getUniform<flat::Matrix4>("vpMatrix");

	m_uiRender.settings.positionAttribute           = m_uiRender.program.getAttribute("position");
	m_uiRender.settings.uvAttribute                 = m_uiRender.program.getAttribute("uv");

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
	
	// level
	loadMap(game, game.mapName);

	// load debug display resources *after* the map is loaded!
	FLAT_DEBUG_ONLY(m_debugDisplay.loadResources(game);)

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
		game.stop();
	}
	
	update(game);
	draw(game);
}

void BaseMapState::exit(Game& game)
{
	map::Map& map = getMap();
	for (entity::Entity* entity : map.getEntities())
	{
		m_entityPool.destroyEntity(entity);
	}
	clearGhostTemplate();
	map.removeAllEntities();
}

void BaseMapState::setModPath(const std::string& modPath)
{
	m_mod.setPath(modPath);
}

bool BaseMapState::loadMap(Game& game, const std::string& mapName)
{
	map::Map& map = getMap();
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
	return map.save(m_mod, game.mapName);
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
		flat::AABB2 tileAABB(tileCenter - flat::Vector2(0.5f), tileCenter + flat::Vector2(0.5f));
		flat::Vector2 mapPositionOnTile = tileCenter + delta;
		if (tileAABB.isInside(mapPositionOnTile))
		{
			mapPosition = mapPositionOnTile;
			mapPosition.x = glm::clamp(mapPosition.x, tileCenter.x - (0.5f - flat::EPSILON), tileCenter.x + (0.5f - flat::EPSILON));
			mapPosition.y = glm::clamp(mapPosition.y, tileCenter.y - (0.5f - flat::EPSILON), tileCenter.y + (0.5f - flat::EPSILON));
			isOnTile = true;
		}
		else
		{
			mapPosition.x = std::min(mapPositionOnTile.x, tileCenter.x + 0.5f + flat::EPSILON);
			mapPosition.y = std::min(mapPositionOnTile.y, tileCenter.y + 0.5f + flat::EPSILON);
		}
	}

	return mapPosition;
}

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
	return m_entityTemplateManager.getResource(game, m_componentRegistry, entityTemplatePath, entityTemplateName);
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

entity::Entity* BaseMapState::spawnEntityAtPosition(Game& game, const std::shared_ptr<const entity::EntityTemplate>& entityTemplate, const flat::Vector3& position, float heading, float elevation, entity::component::ComponentFlags componentFlags)
{
	entity::Entity* entity = createEntity(game, entityTemplate, componentFlags);
	entity->setPosition(position);
	entity->setHeading(heading);
	entity->setElevation(elevation);
	addEntityToMap(entity);
	const float currentTime = m_clock->getTime();
	entity->update(currentTime, 0.f);
	return entity;
}

void BaseMapState::despawnEntity(entity::Entity* entity)
{
	removeEntityFromMap(entity);
	removeFromSelectedEntities(entity);
	destroyEntity(entity);
}

void BaseMapState::despawnEntityAtIndex(int index)
{
	entity::Entity* entity = removeEntityFromMapAtIndex(index);
	removeFromSelectedEntities(entity);
	destroyEntity(entity);
}

void BaseMapState::despawnEntities()
{
	map::Map& map = getMap();
	std::vector<entity::Entity*>& entities = map.getEntities();
	for (int i = static_cast<int>(entities.size() - 1); i >= 0; --i)
	{
		entity::Entity* entity = entities[i];
		if (entity->isMarkedForDelete())
		{
			despawnEntityAtIndex(i);
		}
	}
}

void BaseMapState::setGhostTemplate(Game& game, const std::shared_ptr<const entity::EntityTemplate>& ghostTemplate)
{
	clearGhostTemplate();
	m_ghostTemplate = ghostTemplate;
	m_ghostEntity = createEntity(game, ghostTemplate, entity::component::AllComponents);
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
	map::Map& map = getMap();
	map.addEntity(entity);
}

void BaseMapState::removeEntityFromMap(entity::Entity* entity)
{
	FLAT_ASSERT(entity->getMap() != nullptr);
	map::Map& map = getMap();
	map.removeEntity(entity);
}

entity::Entity* BaseMapState::removeEntityFromMapAtIndex(int index)
{
	map::Map& map = getMap();
	return map.removeEntityAtIndex(index);
}

bool BaseMapState::isMouseOverUi(game::Game& game) const
{
	flat::sharp::ui::RootWidget* root = game.ui->root.get();
	return root->isMouseOver() && root->getCurrentMouseOverWidget().lock() != m_selectionWidget;
}

void BaseMapState::update(game::Game& game)
{
	updateGameView(game);
	updateUi(game);
	m_timerContainer.updateTimers(game.lua->state);
}

void BaseMapState::addGhostEntity(game::Game& game)
{
	if (m_ghostEntity != nullptr && !isMouseOverUi(game) && !isSelecting() && !m_mouseOverEntity.isValid())
	{
		bool isOnTile;
		flat::Vector2 cursorPosition = getCursorMapPosition(game, isOnTile);
		if (isOnTile)
		{
			map::Tile* tile = getMap().getTileIfWalkable(cursorPosition.x, cursorPosition.y);
			if (tile != nullptr)
			{
				entity::component::collision::CollisionComponent* collisionComponent = m_ghostEntity->getComponent<entity::component::collision::CollisionComponent>();
				m_ghostEntity->resetComponents();
				m_ghostEntity->setHeading(0.f);
				m_ghostEntity->setElevation(0.f);
				flat::Vector3 ghostPosition(cursorPosition, tile->getZ());
				m_ghostEntity->setPosition(ghostPosition);
				addEntityToMap(m_ghostEntity);

				if (collisionComponent != nullptr)
				{
					collisionComponent->incDisableLevel();
				}
				m_ghostEntity->update(m_clock->getTime(), 0.f);
				if (collisionComponent != nullptr)
				{
					collisionComponent->decDisableLevel();
				}

				// TODO: clean this shit
				flat::render::Sprite& sprite = const_cast<flat::render::Sprite&>(m_ghostEntity->getSprite());
				flat::video::Color color = sprite.getColor();
				color.a = 0.6f;
				sprite.setColor(color);
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
		float zoom = m_cameraZoom * (1.f + mouse->getWheelMove().y / 5.f);
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
	
	game.video->setClearColor(flat::video::Color::BLACK);
	game.video->clear();
	
	map::Map& map = getMap();
	map.updateTilesNormals();
	addGhostEntity(game);
	getMap().getDisplayManager().sortByDepthAndDraw(game, m_gameView);
	removeGhostEntity(game);
	
#ifdef FLAT_DEBUG
	map.debugDraw(m_debugDisplay);
	m_debugDisplay.drawElements(game, m_gameView);
#endif
	
	// ui
	drawUi(game);
}

void BaseMapState::buildUi(game::Game& game)
{
	// create selection widget
	flat::sharp::ui::WidgetFactory& widgetFactory = game.ui->factory;
	m_selectionWidget = widgetFactory.makeFixedSize(flat::Vector2(1.f, 1.f));
	m_selectionWidget->setPositionPolicy(flat::sharp::ui::Widget::PositionPolicy::BOTTOM_LEFT);
	m_selectionWidget->setBackgroundColor(flat::video::Color(0.f, 8.f, 0.f, 0.3f));
}

void BaseMapState::updateUi(game::Game& game)
{
	flat::sharp::ui::RootWidget* root = game.ui->root.get();
	root->update();
}

void BaseMapState::drawUi(game::Game& game)
{
	m_uiRender.program.use(game.video->window);
	
	m_uiRender.settings.viewProjectionMatrixUniform.set(game.interfaceView.getViewProjectionMatrix());
	
	m_uiRender.settings.modelMatrixUniform.set(flat::Matrix4());
	m_uiRender.settings.colorUniform.set(flat::video::Color(1.0f, 0.0f, 0.0f, 1.0f));
	
	flat::sharp::ui::RootWidget* root = game.ui->root.get();
	root->draw(m_uiRender.settings);
}

void BaseMapState::resetViews(game::Game& game)
{
	const flat::Vector2 windowSize = game.video->window->getSize();

	game.interfaceView.reset();
	game.interfaceView.move(windowSize / 2.0f);
	game.interfaceView.updateProjection();
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

	const entity::Entity* previousMouseOverEntity = m_mouseOverEntity.getEntity();
	const entity::Entity* newMouseOverEntity = nullptr;

	const map::MapObject* mouseOverObject = getMap().getDisplayManager().getObjectAtPosition(viewMousePosition);
	if (mouseOverObject != nullptr)
	{
		if (mouseOverObject->isEntity())
		{
			const entity::Entity* entity = static_cast<const entity::Entity*>(mouseOverObject);
			if (entity->getCanBeSelected())
			{
				newMouseOverEntity = entity;
			}
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

void BaseMapState::setMouseOverColor(const entity::Entity* entity) const
{
	const_cast<flat::render::Sprite&>(entity->getSprite()).setColor(flat::video::Color::GREEN);
}

void BaseMapState::clearMouseOverColor(const entity::Entity* entity) const
{
	flat::video::Color color = flat::video::Color::WHITE;
	if (entity->isSelected())
	{
		color = flat::video::Color::RED;
	}
	const_cast<flat::render::Sprite&>(entity->getSprite()).setColor(color);
}

bool BaseMapState::updateSelectionWidget(Game& game)
{
	if (isMouseOverUi(game))
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

	if (entity::Entity* mouseOverEntity = m_mouseOverEntity.getEntity())
	{
		addToSelectedEntities(mouseOverEntity);
	}
}

void BaseMapState::selectEntitiesOfTypeInScreen(Game& game, const flat::Vector2& mousePosition, bool addToSelection)
{
	if (!addToSelection)
	{
		clearSelection();
	}

	entity::Entity* mouseOverEntity = m_mouseOverEntity.getEntity();
	if (mouseOverEntity != nullptr && mouseOverEntity->getCanBeSelected())
	{
		const std::shared_ptr<const entity::EntityTemplate>& entityTemplate = mouseOverEntity->getTemplate();

		flat::AABB2 screenAABB;
		m_gameView.getScreenAABB(screenAABB);

		const map::DisplayManager& mapDisplayManager = getMap().getDisplayManager();
		std::vector<const map::MapObject*> entitiesInScreen;
		mapDisplayManager.getEntitiesInAABB(screenAABB, entitiesInScreen);

		for (const map::MapObject* mapObject : entitiesInScreen)
		{
			// TODO: fix these casts
			entity::Entity* entity = const_cast<entity::Entity*>(static_cast<const entity::Entity*>(mapObject));
			if (entity->getTemplate() == entityTemplate)
			{
				addToSelectedEntities(entity);
			}
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

	const map::DisplayManager& mapDisplayManager = getMap().getDisplayManager();
	std::vector<const map::MapObject*> entitiesInSelectionWidget;
	mapDisplayManager.getEntitiesInAABB(selectionAABB, entitiesInSelectionWidget);

	for (const map::MapObject* mapObject : entitiesInSelectionWidget)
	{
		// TODO: fix these casts
		entity::Entity* entity = const_cast<entity::Entity*>(static_cast<const entity::Entity*>(mapObject));
		if (!entity->getCanBeSelected())
		{
			continue;
		}

		// check that the sprite origin actually is in the AABB
		if (selectionAABB.isInside(entity->getSprite().getPosition()))
		{
			addToSelectedEntities(entity);
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

void BaseMapState::addToSelectedEntities(entity::Entity * entity)
{
	if (!entity->isSelected())
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


