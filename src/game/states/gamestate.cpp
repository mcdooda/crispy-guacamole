#include <iostream>
#include "gamestate.h"
#include "../game.h"
#include "../map/tile.h"

namespace game
{
namespace states
{

void GameState::enter(flat::state::Agent* agent)
{
	game::Game* game = agent->to<game::Game>();
	
	// init lua first
	m_luaState = flat::lua::open();
	
	// rendering settings
	m_program.load("data/shaders/ui.frag", "data/shaders/ui.vert");
	
	m_programRenderSettings.textureUniform              = m_program.getUniform("objectTexture");
	m_programRenderSettings.colorUniform                = m_program.getUniform("color");
	m_programRenderSettings.modelMatrixUniform          = m_program.getUniform("modelMatrix");
	m_programRenderSettings.viewProjectionMatrixUniform = m_program.getUniform("vpMatrix");
	m_programRenderSettings.positionAttribute           = m_program.getAttribute("position");
	m_programRenderSettings.uvAttribute                 = m_program.getAttribute("uv");
	
	// load mod
	m_mod.setPath(game->argGetString(1));
	m_mod.readConfig(m_luaState);
	
	// level
	m_map.load(game, m_mod);
	flat::geometry::Vector3 cameraCenter(m_map.getWidth() / 2, m_map.getHeight() / 2, 0.f);
	
	// reset view
	const flat::geometry::Vector2& windowSize = game->video->window->getSize();
	m_gameView.updateProjection(windowSize);
	setCameraCenter(cameraCenter);
}

void GameState::execute(flat::state::Agent* agent)
{
	game::Game* game = agent->to<game::Game>();
	
	update(game);
	draw(game);
}

void GameState::exit(flat::state::Agent* agent)
{
	flat::lua::close(m_luaState);
	m_luaState = nullptr;
}

void GameState::update(game::Game* game)
{
	updateGameView(game);
}

void GameState::updateGameView(game::Game* game)
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

void GameState::setCameraCenter(const flat::geometry::Vector3& cameraCenter)
{
	flat::geometry::Vector2 center2d = m_map.getXAxis() * cameraCenter.x
	                                 + m_map.getYAxis() * cameraCenter.y
	                                 + m_map.getZAxis() * cameraCenter.z;
	m_gameView.reset();
	m_gameView.revertY();
	m_gameView.move(center2d);
}

void GameState::draw(game::Game* game)
{
	m_program.use(game->video->window);
	
	game->video->setClearColor(flat::video::Color::BLACK);
	game->video->clear();
	
	m_programRenderSettings.viewProjectionMatrixUniform.setMatrix4(m_gameView.getViewProjectionMatrix());
	
	m_mapDisplayManager.clearAll();
	m_map.drawTiles(m_mapDisplayManager, m_gameView);
	m_mapDisplayManager.sortByDepthAndDraw(m_programRenderSettings, m_gameView.getViewProjectionMatrix());
	
	{
		flat::geometry::Matrix4 transform = game->interfaceView.getViewProjectionMatrix();
		transform.setInverse();
		flat::geometry::Vector2 mouseWorldPosition = m_gameView.getViewProjectionMatrix() * (transform * game->input->mouse->getPosition());
		flat::geometry::Rectangle r(mouseWorldPosition - flat::geometry::Vector2(10.0f, 10.0f), flat::geometry::Vector2(20.0f, 20.0f));
		m_programRenderSettings.colorUniform.setColor(flat::video::Color(0.0f, 1.0f, 0.0f, 1.0f));
		r.draw(m_programRenderSettings.positionAttribute, m_programRenderSettings.uvAttribute);
	}
	
	// ui
	m_programRenderSettings.viewProjectionMatrixUniform.setMatrix4(game->interfaceView.getViewProjectionMatrix());
	
	m_programRenderSettings.modelMatrixUniform.setMatrix4(flat::geometry::Matrix4());
	m_programRenderSettings.colorUniform.setColor(flat::video::Color(1.0f, 0.0f, 0.0f, 1.0f));
	
	{
		flat::geometry::Rectangle r(flat::geometry::Vector2(2.0f, 2.0f), flat::geometry::Vector2(10.0f, 10.0f));
		r.draw(m_programRenderSettings.positionAttribute, m_programRenderSettings.uvAttribute);
	}
	{
		const flat::geometry::Vector2& windowSize = game->video->window->getSize();
		flat::geometry::Rectangle r(windowSize - flat::geometry::Vector2(12.0f, 12.0f), flat::geometry::Vector2(10.0f, 10.0f));
		r.draw(m_programRenderSettings.positionAttribute, m_programRenderSettings.uvAttribute);
	}
	
	{
		flat::geometry::Vector2 mouseWorldPosition = m_gameView.getRelativePosition(game->input->mouse->getPosition(), game->video->window->getSize());
		flat::geometry::Rectangle r(mouseWorldPosition - flat::geometry::Vector2(5.0f, 5.0f), flat::geometry::Vector2(10.0f, 10.0f));
		m_programRenderSettings.colorUniform.setColor(flat::video::Color(0.0f, 0.0f, 1.0f, 1.0f));
		r.draw(m_programRenderSettings.positionAttribute, m_programRenderSettings.uvAttribute);
	}
}

} // states
} // game


