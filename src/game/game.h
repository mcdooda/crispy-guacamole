#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <flat/game.h>

namespace game
{

class Game : public flat::Game
{
	public:
		Game(const std::vector<std::string>& args);
		~Game() override;
		
		void setStates() override;
		void checkArgs() override;
		void openWindow() override;
		
	public:
		// views
		flat::video::View interfaceView;
};

} // game

#endif // GAME_GAME_H



