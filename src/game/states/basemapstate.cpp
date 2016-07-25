#include <iostream>
#include <flat.h>
#include "basemapstate.h"
#include "../game.h"
#include "../map/tile.h"
#include "../timer/lua/timer.h"

namespace game
{
namespace states
{

void BaseMapState::enter(flat::state::Agent* agent)
{
	game::Game* game = agent->to<game::Game>();
	
	// init lua first
	m_luaState = flat::lua::open(game);
	
	//TODO lua_pushcclosure...
	flat::time::lua::open(m_luaState);
	flat::sharp::ui::lua::open(m_luaState);
	timer::lua::open(m_luaState);
	
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
	m_uiProgramRenderSettings.colorUniform                = m_uiProgram.getUniform("color");
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
	const flat::geometry::Vector2& mousePosition = game->input->mouse->getPosition();
	const flat::geometry::Vector2& windowSize = game->video->window->getSize();
	const float screenBorderWidth = 5.f;
	
	int tileWidth = m_map.getTileWidth();
	int tileHeight = m_map.getTileHeight();
	
	flat::geometry::Vector2 move;
	
	if (mousePosition.x < screenBorderWidth)
		move.x = tileWidth;
		
	else if (mousePosition.x >= windowSize.x - screenBorderWidth)
		move.x = -tileWidth;
		
		
	if (mousePosition.y < screenBorderWidth)
		move.y = tileHeight;
		
	else if (mousePosition.y >= windowSize.y - screenBorderWidth)
		move.y = -tileHeight;
	
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
	m_gameView.move(move * game->time->getFrameTime() * cameraSpeed);
	
	if (game->input->window->isResized())
		m_gameView.updateProjection(windowSize);
}

void BaseMapState::setCameraCenter(const flat::geometry::Vector3& cameraCenter)
{
	flat::geometry::Vector2 center2d = m_map.getXAxis() * cameraCenter.x
	                                 + m_map.getYAxis() * cameraCenter.y
	                                 + m_map.getZAxis() * cameraCenter.z;
	m_gameView.reset();
	m_gameView.revertY();
	m_gameView.move(center2d);
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

} // states
} // game


