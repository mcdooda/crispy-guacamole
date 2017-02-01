#include <iostream>
#include <flat.h>
#include "basemapstate.h"
#include "editor/lua/editor.h"
#include "../game.h"
#include "../timer/lua/timer.h"
#include "../map/map.h"
#include "../map/tile.h"
#include "../map/tiletemplate.h"
#include "../map/proptemplate.h"
#include "../map/lua/map.h"
#include "../entity/lua/entity.h"
#include "../entity/component/lua/componentregistry.h"
#include "../entity/faction/lua/faction.h"
#include "../entity/entitytemplate.h"
#include "../mod/lua/mod.h"

namespace game
{
namespace states
{

BaseMapState::BaseMapState() :
	m_entityPool(m_componentRegistry)
{
}

void BaseMapState::enter(Game& game)
{
	game.video->window->setTitle("Crispy guacamole");
	
	// init lua first
	lua_State* L = game.lua->state;
	{
		timer::lua::open(L);
		entity::lua::open(L);
		entity::component::lua::open(L);
		entity::faction::lua::open(L, m_mod.getFactionsConfigPath());
		mod::lua::open(L);
		map::lua::open(L);
		editor::lua::open(L);
	}
	
	// ui
	buildUi(game);
	
	// rendering settings
	m_spriteProgram.load("data/shaders/spritebatch.frag", "data/shaders/spritebatch.vert");
	
	m_spriteProgramRenderSettings.textureUniform              = m_spriteProgram.getUniform<flat::video::Texture>("objectTexture");
	m_spriteProgramRenderSettings.viewProjectionMatrixUniform = m_spriteProgram.getUniform<flat::Matrix4>("vpMatrix");

	m_spriteProgramRenderSettings.positionAttribute           = m_spriteProgram.getAttribute("position");
	m_spriteProgramRenderSettings.uvAttribute                 = m_spriteProgram.getAttribute("uv");
	m_spriteProgramRenderSettings.colorAttribute              = m_spriteProgram.getAttribute("color");
	
	m_uiProgram.load("data/shaders/ui.frag", "data/shaders/ui.vert");
	
	m_uiProgramRenderSettings.textureUniform              = m_uiProgram.getUniform<flat::video::Texture>("objectTexture");
	m_uiProgramRenderSettings.textureGivenUniform         = m_uiProgram.getUniform<bool>("textureGiven");
	m_uiProgramRenderSettings.colorUniform                = m_uiProgram.getUniform<flat::video::Color>("color");
	m_uiProgramRenderSettings.secondaryColorUniform       = m_uiProgram.getUniform<flat::video::Color>("secondaryColor");
	m_uiProgramRenderSettings.modelMatrixUniform          = m_uiProgram.getUniform<flat::Matrix4>("modelMatrix");
	m_uiProgramRenderSettings.viewProjectionMatrixUniform = m_uiProgram.getUniform<flat::Matrix4>("vpMatrix");

	m_uiProgramRenderSettings.positionAttribute           = m_uiProgram.getAttribute("position");
	m_uiProgramRenderSettings.uvAttribute                 = m_uiProgram.getAttribute("uv");
	
	// level
	loadMap(game, game.mapName);

	// load debug display resources *after* the map is loaded!
	FLAT_DEBUG_ONLY(m_debugDisplay.loadResources(game);)
	
	// reset view
	const flat::Vector2& windowSize = game.video->window->getSize();
	m_gameView.updateProjection(windowSize);
	m_cameraZoom = 1.f;

	const map::Map& map = getMap();
	int minX, maxX, minY, maxY;
	map.getBounds(minX, maxX, minY, maxY);
	flat::Vector3 cameraCenter((maxX + minX) / 2.f, (maxY + minY) / 2.f, 0.f);
	setCameraCenter(cameraCenter);

	resetViews(game);
}

void BaseMapState::execute(Game& game)
{
	if (game.input->window->isResized())
		resetViews(game);

	if (game.input->keyboard->isJustPressed(K(ESCAPE)))
		game.stop();
	
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
	map.removeAllEntities();

	m_ui.reset();
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

flat::Vector2 BaseMapState::getCursorMapPosition(game::Game& game)
{
	const flat::Vector2& cursorPosition = game.input->mouse->getPosition();
	const flat::Vector2& windowSize = game.video->window->getSize();
	flat::Vector2 gameViewPosition = m_gameView.getRelativePosition(cursorPosition, windowSize);

	const map::Map& map = getMap();
	const flat::Vector2& xAxis = map.getXAxis();
	const flat::Vector2& yAxis = map.getYAxis();

	flat::Vector2 mapPosition;

	mapPosition.x = (gameViewPosition.x * yAxis.y - gameViewPosition.y * yAxis.x) / (xAxis.x * yAxis.y - xAxis.y * yAxis.x);
	mapPosition.y = (gameViewPosition.y * xAxis.x - gameViewPosition.x * xAxis.y) / (yAxis.y * xAxis.x - yAxis.x * xAxis.y);

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

entity::Entity* BaseMapState::spawnEntityAtPosition(Game& game, const std::shared_ptr<const entity::EntityTemplate>& entityTemplate, const flat::Vector3& position, float heading, float elevation)
{
	entity::component::ComponentFlags componentsFilter = getComponentsFilter();
	entity::Entity* entity = m_entityPool.createEntity(entityTemplate, m_componentRegistry, componentsFilter);
	entity->setPosition(position);
	entity->setHeading(heading);
	entity->setElevation(elevation);
	entity->initComponents();
	map::Map& map = getMap();
	map.addEntity(entity);
	m_entities.push_back(entity);
	const float currentTime = game.time->getTime();
	entity->update(currentTime, 0.f);
	return entity;
}

void BaseMapState::despawnEntity(entity::Entity* entity)
{
	map::Map& map = getMap();
	map.removeEntity(entity);
	if (entity->isSelected())
	{
		// remove from selected entities
		std::vector<entity::Entity*>::iterator it = std::find(m_selectedEntities.begin(), m_selectedEntities.end(), entity);
		FLAT_ASSERT(it != m_selectedEntities.end());
		m_selectedEntities.erase(it);
	}
	m_entityPool.destroyEntity(entity);
}

void BaseMapState::despawnEntities()
{
	for (int i = static_cast<int>(m_entities.size()) - 1; i >= 0; --i)
	{
		entity::Entity* entity = m_entities[i];
		if (entity->isMarkedForDelete())
		{
			// swap the current and the last element (if current != last)
			if (i < static_cast<int>(m_entities.size()) - 1)
			{
				entity::Entity* lastEntity = m_entities.back();
				m_entities[i] = lastEntity;
			}
			m_entities.pop_back();
			despawnEntity(entity);
		}
	}
}

void BaseMapState::update(game::Game& game)
{
	updateGameView(game);
	updateUi(game);
	float currentTime = game.time->getTime();
	game.timerContainer.updateTimers(game.lua->state, currentTime);
}

void BaseMapState::updateGameView(game::Game& game)
{
	const flat::input::Keyboard* keyboard = game.input->keyboard;
	const flat::input::Mouse* mouse = game.input->mouse;

	const flat::Vector2& windowSize = game.video->window->getSize();
	
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
	m_cameraCenter2d += move * game.time->getActualFrameTime() * cameraSpeed;
	updateCameraView();

	if (mouse->wheelJustMoved() && keyboard->isPressed(K(LCTRL)))
	{
		float zoom = m_cameraZoom * (1.f + mouse->getWheelMove().y / 5.f);
		setCameraZoom(zoom);
	}
	
	if (game.input->window->isResized())
		m_gameView.updateProjection(windowSize);
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
	m_spriteProgram.use(game.video->window);
	
	game.video->setClearColor(flat::video::Color::BLACK);
	game.video->clear();
	
	m_spriteProgramRenderSettings.viewProjectionMatrixUniform.set(m_gameView.getViewProjectionMatrix());
	
	m_mapDisplayManager.clearAll();
	const map::Map& map = getMap();
	map.drawTiles(m_mapDisplayManager, m_gameView);
	m_mapDisplayManager.sortByDepthAndDraw(m_spriteProgramRenderSettings, m_gameView.getViewProjectionMatrix());
	
#ifdef FLAT_DEBUG
	map.debugDraw(m_debugDisplay);
	m_debugDisplay.drawElements(game, m_gameView);
#endif
	
	// ui
	drawUi(game);
}

void BaseMapState::buildUi(game::Game& game)
{
	flat::sharp::ui::WidgetFactory widgetFactory(game);

	m_ui = widgetFactory.makeRoot();
	
	flat::sharp::ui::lua::setRootWidget(game.lua->state, m_ui.get()); // TODO: move to flat

	// create selection widget
	m_selectionWidget = widgetFactory.makeFixedSize(flat::Vector2(1.f, 1.f));
	m_selectionWidget->setPositionPolicy(flat::sharp::ui::Widget::PositionPolicy::BOTTOM_LEFT);
	m_selectionWidget->setBackgroundColor(flat::video::Color(0.f, 8.f, 0.f, 0.3f));
}

void BaseMapState::updateUi(game::Game& game)
{
	m_ui->update();
}

void BaseMapState::drawUi(game::Game& game)
{
	m_uiProgram.use(game.video->window);
	
	m_uiProgramRenderSettings.viewProjectionMatrixUniform.set(game.interfaceView.getViewProjectionMatrix());
	
	m_uiProgramRenderSettings.modelMatrixUniform.set(flat::Matrix4());
	m_uiProgramRenderSettings.colorUniform.set(flat::video::Color(1.0f, 0.0f, 0.0f, 1.0f));
	
	m_ui->draw(m_uiProgramRenderSettings);
}

void BaseMapState::resetViews(game::Game& game)
{
	const flat::Vector2 windowSize = game.video->window->getSize();

	game.interfaceView.reset();
	game.interfaceView.move(windowSize / 2.0f);
	game.interfaceView.updateProjection(windowSize);
}

entity::component::ComponentFlags BaseMapState::getComponentsFilter() const
{
	return entity::component::AllComponents;
}

bool BaseMapState::updateSelectionWidget(Game& game)
{
	const flat::input::Mouse* mouse = game.input->mouse;
	const flat::input::Keyboard* keyboard = game.input->keyboard;
	const flat::Vector2& mousePosition = mouse->getPosition();

	if (mouse->isJustPressed(M(LEFT)))
	{
		m_mouseDownPosition = mousePosition;
	}

	if (mouse->isPressed(M(LEFT)) && mouse->justMoved())
	{
		if (!isSelecting())
		{
			// begin selection
			m_ui->addChild(m_selectionWidget);
		}

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
	else if (mouse->isJustReleased(M(LEFT)))
	{
		if (isSelecting())
		{
			flat::sharp::ui::Widget* selectionWidget = m_selectionWidget.get();
			const flat::Vector2& bottomLeft = selectionWidget->getPosition();
			flat::Vector2 topRight = bottomLeft + selectionWidget->getSize();

			const bool shiftPressed = keyboard->isPressed(K(LSHIFT));
			updateSelectedEntities(game, bottomLeft, topRight, shiftPressed);

			selectionWidget->removeFromParent();
			return true;
		}
	}

	return false;
}

void BaseMapState::updateSelectedEntities(Game& game, const flat::Vector2& bottomLeft, const flat::Vector2& topRight, bool addToSelection)
{
	const flat::Vector2& windowSize = game.video->window->getSize();
	const flat::Vector2 viewBottomLeft = m_gameView.getRelativePosition(bottomLeft, windowSize);
	const flat::Vector2 viewTopRight = m_gameView.getRelativePosition(topRight, windowSize);

	if (!addToSelection)
	{
		m_selectedEntities.clear();
	}

	for (entity::Entity* entity : m_entities) // TODO: optimize this
	{
		if (!entity->canBeSelected())
		{
			continue;
		}

		const flat::render::Sprite& sprite = entity->getSprite(); // TODO: discard entities with no sprite
		const flat::Vector2& spritePosition = sprite.getPosition();
		if (viewBottomLeft.x <= spritePosition.x && spritePosition.x <= viewTopRight.x
			&& viewTopRight.y <= spritePosition.y && spritePosition.y <= viewBottomLeft.y) // y is flipped in the game view
		{
			if (!addToSelection || !entity->isSelected())
			{
				m_selectedEntities.push_back(entity);
			}

			if (!entity->isSelected())
			{
				entity->setSelected(true);
			}
		}
		else if (!addToSelection && entity->isSelected())
		{
			entity->setSelected(false);
		}
	}
}

void BaseMapState::clearSelection(Game& game)
{
	for (entity::Entity* entity : m_selectedEntities)
	{
		entity->setSelected(false);
	}
	m_selectedEntities.clear();
}

} // states
} // game


