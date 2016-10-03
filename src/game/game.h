#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <flat/game.h>
#include "timer/timercontainer.h"

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

		// make Game& usable as a Resource constructor parameter
		bool operator<(const Game& other) { return this < &other; }
		
	public:
		// mod
		std::string modPath;
		std::string mapName;
		
		// views
		flat::video::View interfaceView;
		
		// timers
		timer::TimerContainer timerContainer;
		
		// GAME -> GameState, EDITOR -> EditorState
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



