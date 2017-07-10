#include "game.h"
#include "states/gamestate.h"
#include "states/mapeditorstate.h"
#include "states/entityeditorstate.h"

namespace game
{

Game::Game(const std::vector<std::string>& args) : flat::Game(args),
	interfaceView(video->window),
	mode(Mode::GAME),
	fullscreen(true)
{
	
}

Game::~Game()
{
	
}

void Game::setStates()
{
	states::BaseMapState* state = nullptr;
	
	switch (mode)
	{
		case Mode::GAME:
		state = new states::GameState();
		break;
		
		case Mode::MAPEDITOR:
		if (mapName.empty())
		{
			wrongArguments();
		}
		state = new states::MapEditorState();
		break;

		case Mode::ENTITYEDITOR:
		if (entityName.empty() || mapName.empty())
		{
			wrongArguments();
		}
		state = new states::EntityEditorState();
		break;
		
		default:
		FLAT_ASSERT_MSG(false, "Unknown mode %u", mode);
	}
	
	state->setModPath(modPath);
	getStateMachine().setState(state);
}

void Game::checkArgs()
{
	for (int i = 1; isArg(i); ++i)
	{
		std::string arg = argGetString(i);
		switch (arg[0])
		{
			case '-':
			{
				switch (arg[1])
				{
					case 'w':
					fullscreen = false;
					break;

					case 'g':
					mode = Mode::GAME;
					modPath = argGetString(++i);
					mapName = argGetString(++i);
					break;
					
					case 'm':
					mode = Mode::MAPEDITOR;
					modPath = argGetString(++i);
					mapName = argGetString(++i);
					break;

					case 'e':
					mode = Mode::ENTITYEDITOR;
					modPath = argGetString(++i);
					mapName = argGetString(++i);
					entityName = argGetString(++i);
					break;
					
					default:
					std::cerr << "Unhandled argument '" << arg << "'" << std::endl;
					break;
				}
			}
			break;
			
			default:
				std::cerr << "Unhandled argument '" << arg << "'" << std::endl;
		}
	}
	
	if (modPath.empty())
	{
		wrongArguments();
	}
}

void Game::openWindow()
{
	if (fullscreen)
	{
		video->window->open(video->window->getDesktopSize(), true, true);
	}
	else
	{
		Super::openWindow();
	}
}

void Game::wrongArguments()
{
	std::cerr << "You must specify a mod path" << std::endl
		<< "Other options:" << std::endl
		<< "\t-w\t\t\twindowed" << std::endl
		<< "\t-g <map>\t\tgame mode" << std::endl
		<< "\t-m <map>\t\tmap editor mode" << std::endl
		<< "\t-e <entity> <map>\tentity editor mode" << std::endl;
	FLAT_BREAK();
	exit(1);
}

} // game



