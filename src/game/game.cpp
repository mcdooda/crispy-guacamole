#include "game.h"
#include "states/globalstate.h"
#include "states/gamestate.h"

namespace game
{

Game::Game(const std::vector<std::string>& args) : flat::Game(args)
{
	
}

Game::~Game()
{
	
}

void Game::setStates()
{
	getStateMachine()->setGlobalState(new states::GlobalState());
	getStateMachine()->setState(new states::GameState());
}

void Game::checkArgs()
{
	argCheckString(1);
}

void Game::openWindow()
{
	video->window->open(video->window->getDesktopSize(), true, true);
}

} // game



