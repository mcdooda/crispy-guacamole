#include "game.h"
#include "states/globalstate.h"
#include "states/gamestate.h"
#include "states/editorstate.h"

namespace game
{

Game::Game(const std::vector<std::string>& args) : flat::Game(args),
	mode(Mode::GAME),
	fullscreen(true)
{
	
}

Game::~Game()
{
	
}

void Game::setStates()
{
	getStateMachine()->setGlobalState(new states::GlobalState());
	
	states::BaseMapState* state = nullptr;
	
	switch (mode)
	{
		case Mode::GAME:
		state = new states::GameState();
		break;
		
		case Mode::EDITOR:
		state = new states::EditorState();
		break;
		
		default:
		FLAT_ASSERT_MSG(false, "Unknown mode %u", mode);
	}
	
	state->setModPath(modPath);
	getStateMachine()->setState(state);
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
					
					case 'e':
					mode = Mode::EDITOR;
					break;
					
					default:
					std::cerr << "Unhandled argument '" << arg << "'" << std::endl;
					break;
				}
			}
			break;
			
			default:
			{
				if (modPath.empty())
				{
					modPath = arg;
				}
				else if (mapName.empty())
				{
					mapName = arg;
				}
				else
				{
					std::cerr << "Unhandled argument '" << arg << "' (mod path already given)" << std::endl;
				}
			}
		}
	}
	
	if (modPath.empty())
	{
		std::cerr << "You must specify a mod path" << std::endl
		          << "Other options:" << std::endl
		          << "\t-w\twindowed" << std::endl
		          << "\t-e\teditor mode" << std::endl;
		FLAT_BREAK();
		exit(1);
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

} // game



