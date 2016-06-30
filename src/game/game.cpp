#include "game.h"

namespace game
{

Game::Game(const std::vector<std::string>& args) : flat::Game(args)
{
	
}

Game::~Game()
{
	
}

void Game::checkArgs()
{
	argCheckString(1);
}

} // game



