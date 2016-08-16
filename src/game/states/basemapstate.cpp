#include <iostream>
#include <flat.h>
#include "basemapstate.h"
#include "../game.h"
#include "../map/tile.h"
#include "../timer/lua/timer.h"
#include "../entity/entitytemplate.h"
#include "../entity/lua/entity.h"

namespace game
{
namespace states
{

void BaseMapState::enter(flat::state::Agent* agent)
{
	game::Game* game = agent->to<game::Game>();
	
	// init lua first
	m_luaState = flat::lua::open(game);
	
	flat::time::lua::open(m_luaState);
	flat::input::lua::mouse::open(m_luaState);
	flat::sharp::ui::lua::open(m_luaState);
	timer::lua::open(m_luaState);
	entity::lua::open(m_luaState);
	
	// ui
	buildUi(game);
	
	// rendering settings
	m_spriteProgram.load("data/shaders/sprite.frag", "data/shaders/sprite.vert");
	
	m_spriteProgramRenderSettings.textureUniform              = m_spriteProgram.getUniform("objectTexture");
	m_spriteProgramRenderSettings.colorUniform                = m_spriteProgram.getUniform("color");
	m_spriteProgramRenderSettings.modelMatrixUniform          = m_spriteProgram.getUniform("modelMatrix");
	m_spriteProgramRenderSettings.viewProjectionMatrixUniform = m_spriteProgram.getUniform("vpMatrix");
	m_spriteProgramRenderSettings.positionAttribute           = m_spriteProgram.getAttribute("position");
	m_spriteProgramRenderSettings.uvAttribute                 = m_spriteProgram.getAttribute("uv");
	
	m_uiProgram.load("data/shaders/ui.frag", "data/shaders/ui.vert");
	
	m_uiProgramRenderSettings.textureUniform              = m_uiProgram.getUniform("objectTexture");
	m_uiProgramRenderSettings.textureGivenUniform         = m_uiProgram.getUniform("textureGiven");
	m_uiProgramRenderSettings.colorUniform                = m_uiProgram.getUniform("color");
	m_uiProgramRenderSettings.secondaryColorUniform       = m_uiProgram.getUniform("secondaryColor");
	m_uiProgramRenderSettings.modelMatrixUniform          = m_uiProgram.getUniform("modelMatrix");
	m_uiProgramRenderSettings.viewProjectionMatrixUniform = m_uiProgram.getUniform("vpMatrix");
	m_uiProgramRenderSettings.positionAttribute           = m_uiProgram.getAttribute("position");
	m_uiProgramRenderSettings.uvAttribute                 = m_uiProgram.getAttribute("uv");
	
	// load mod
	m_mod.readConfig(m_luaState);
	
	// level
	m_map.load(game, m_mod);
	flat::geometry::Vector3 cameraCenter(m_map.getWidth() / 2, m_map.getHeight() / 2, 0.f);
	
	// reset view
	const flat::geometry::Vector2& windowSize = game->video->window->getSize();
	m_gameView.updateProjection(windowSize);
	m_cameraZoom = 1.f;
	setCameraCenter(cameraCenter);
}

void BaseMapState::execute(flat::state::Agent* agent)
{
	game::Game* game = agent->to<game::Game>();
	
	update(game);
	draw(game);
}

void BaseMapState::exit(flat::state::Agent* agent)
{
	flat::sharp::ui::lua::close(m_luaState);
	flat::lua::close(m_luaState);
	m_luaState = nullptr;
}

void BaseMapState::setModPath(const std::string& modPath)
{
	m_mod.setPath(modPath);
}

void BaseMapState::update(game::Game* game)
{
	updateGameView(game);
	updateUi(game);
	float currentTime = game->time->getTime();
	game->timerContainer.updateTimers(m_luaState, currentTime);
}

void BaseMapState::updateGameView(game::Game* game)
{
	const flat::geometry::Vector2& windowSize = game->video->window->getSize();
	
	const int tileWidth = m_map.getTileWidth();
	const int tileHeight = m_map.getTileHeight();
	
	flat::geometry::Vector2 move;
	
	/*
	const flat::geometry::Vector2& mousePosition = game->input->mouse->getPosition();
	const float screenBorderWidth = 5.f;
	
	if (mousePosition.x < screenBorderWidth)
		move.x = tileWidth;
		
	else if (mousePosition.x >= windowSize.x - screenBorderWidth)
		move.x = -tileWidth;
		
		
	if (mousePosition.y < screenBorderWidth)
		move.y = tileHeight;
		
	else if (mousePosition.y >= windowSize.y - screenBorderWidth)
		move.y = -tileHeight;
	*/
	
	// keyboard wins over mouse
	const flat::input::Keyboard* keyboard = game->input->keyboard;
	bool leftPressed = keyboard->isPressed(K(LEFT));
	bool rightPressed = keyboard->isPressed(K(RIGHT));
	bool upPressed = keyboard->isPressed(K(UP));
	bool downPressed = keyboard->isPressed(K(DOWN));
	
	if (leftPressed && !rightPressed)
		move.x = tileWidth;
		
	else if (rightPressed && !leftPressed)
		move.x = -tileWidth;
		
		
	if (upPressed && !downPressed)
		move.y = -tileHeight;
		
	else if (downPressed && !upPressed)
		move.y = tileHeight;
	
	const float cameraSpeed = 20.f;
	m_cameraCenter2d += move * game->time->getFrameTime() * cameraSpeed;
	updateCameraView();
	
	if (game->input->mouse->wheelJustMoved())
	{
		float zoom = m_cameraZoom * (1.f + game->input->mouse->getWheelMove().y / 5.f);
		setCameraZoom(zoom);
	}
	
	if (game->input->window->isResized())
		m_gameView.updateProjection(windowSize);
}

void BaseMapState::setCameraCenter(const flat::geometry::Vector3& cameraCenter)
{
	m_cameraCenter2d = m_map.getXAxis() * cameraCenter.x
	                 + m_map.getYAxis() * cameraCenter.y
	                 + m_map.getZAxis() * cameraCenter.z;
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
	m_gameView.move(m_cameraCenter2d);
	m_gameView.zoom(m_cameraZoom);
}

void BaseMapState::draw(game::Game* game)
{
	// map
	m_spriteProgram.use(game->video->window);
	
	game->video->setClearColor(flat::video::Color::BLACK);
	game->video->clear();
	
	m_spriteProgramRenderSettings.viewProjectionMatrixUniform.setMatrix4(m_gameView.getViewProjectionMatrix());
	
	m_mapDisplayManager.clearAll();
	m_map.drawTiles(m_mapDisplayManager, m_gameView);
	m_mapDisplayManager.sortByDepthAndDraw(m_spriteProgramRenderSettings, m_gameView.getViewProjectionMatrix());
	
	// ui
	drawUi(game);
}

void BaseMapState::buildUi(game::Game* game)
{
	flat::sharp::ui::WidgetFactory widgetFactory(*game);

	flat::sharp::ui::RootWidget* ui = widgetFactory.makeRoot();
	m_ui.reset(ui);
	
	flat::sharp::ui::lua::setRootWidget(m_luaState, ui);
}

void BaseMapState::updateUi(game::Game* game)
{
	flat::sharp::ui::RootWidget* ui = m_ui.get();

	if (game->input->window->isResized())
		ui->fullLayout();

	ui->updateInput();
}

void BaseMapState::drawUi(game::Game* game)
{
	m_uiProgram.use(game->video->window);
	
	m_uiProgramRenderSettings.viewProjectionMatrixUniform.setMatrix4(game->interfaceView.getViewProjectionMatrix());
	
	m_uiProgramRenderSettings.modelMatrixUniform.setMatrix4(flat::geometry::Matrix4());
	m_uiProgramRenderSettings.colorUniform.setColor(flat::video::Color(1.0f, 0.0f, 0.0f, 1.0f));
	
	m_ui->draw(m_uiProgramRenderSettings);
}

std::shared_ptr<const entity::EntityTemplate> BaseMapState::getEntityTemplate(game::Game* game, const std::string& entityTemplateName) const
{
	std::string entityTemplatePath = m_mod.getEntityTemplatePath(entityTemplateName);
	return m_entityTemplateManager.getResource(game, m_luaState, entityTemplatePath);
}

flat::geometry::Vector2 BaseMapState::getCursorMapPosition(game::Game* game)
{
	const flat::geometry::Vector2& cursorPosition = game->input->mouse->getPosition();
	const flat::geometry::Vector2& windowSize = game->video->window->getSize();
	flat::geometry::Vector2 gameViewPosition = m_gameView.getRelativePosition(cursorPosition, windowSize);
	
	const flat::geometry::Vector2& xAxis = m_map.getXAxis();
	const flat::geometry::Vector2& yAxis = m_map.getYAxis();
	
	flat::geometry::Vector2 mapPosition;
	
	mapPosition.x = (gameViewPosition.x * yAxis.y - gameViewPosition.y * yAxis.x) / (xAxis.x * yAxis.y - xAxis.y * yAxis.x);
	mapPosition.y = (gameViewPosition.y * xAxis.x - gameViewPosition.x * xAxis.y) / (yAxis.y * xAxis.x - yAxis.x * xAxis.y);
	
	return mapPosition;
}

} // states
} // game


