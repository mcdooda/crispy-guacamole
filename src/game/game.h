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
		Game(const Game&) = delete;
		void operator=(const Game&) = delete;
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
		std::string entityName;
		
		// views
		flat::video::View interfaceView;
		
		// GAME -> GameState, MAPEDITOR -> MapEditorState, ENTITYEDITOR -> EntityEditorState
		enum class Mode : uint8_t
		{
			GAME,
			MAPEDITOR,
			ENTITYEDITOR,
		};
		Mode mode;
		
		bool fullscreen;

	private:
		static void wrongArguments();
};

} // game

#endif // GAME_GAME_H



