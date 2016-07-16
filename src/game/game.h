#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <flat/game.h>

namespace game
{

class Game : public flat::Game
{
	typedef flat::Game Super;
	public:
		Game(const std::vector<std::string>& args);
		~Game() override;
		
		void setStates() override;
		void checkArgs() override;
		void openWindow() override;
		
	public:
		// mod
		std::string modPath;
		
		// views
		flat::video::View interfaceView;
		
		enum class Mode : uint8_t
		{
			GAME,
			EDITOR
		};
		Mode mode;
		
		bool fullscreen;
};

} // game

#endif // GAME_GAME_H



