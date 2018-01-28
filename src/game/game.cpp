#include "game.h"
#include "states/selectmapstate.h"
#include "states/gamestate.h"
#include "states/mapeditorstate.h"
#include "states/entityeditorstate.h"

namespace game
{

Game::Game(const std::vector<std::string>& args) : flat::Game(args),
	interfaceView(video->window),
	mode(Mode::NONE),
	fullscreen(false)
{
	
}

Game::~Game()
{
	
}

void Game::setStates()
{
	std::unique_ptr<states::BaseState> state = nullptr;
	
	switch (mode)
	{
		case Mode::NONE:
		state.reset(new states::SelectMapState());
		break;

		case Mode::GAME:
		{
			if (modPath.empty() || mapName.empty())
			{
				wrongArguments();
			}
			states::GameState* gameState = new states::GameState();
			gameState->setModPath(modPath);
			state.reset(gameState);
		}
		break;
		
		case Mode::MAPEDITOR:
		{
			if (modPath.empty() || mapName.empty())
			{
				wrongArguments();
			}
			states::MapEditorState* mapEditorState = new states::MapEditorState();
			mapEditorState->setModPath(modPath);
			state.reset(mapEditorState);
		}
		break;

		case Mode::ENTITYEDITOR:
		{
			if (modPath.empty() || mapName.empty() || entityName.empty())
			{
				wrongArguments();
			}
			states::EntityEditorState* entityEditorState = new states::EntityEditorState();
			entityEditorState->setModPath(modPath);
			state.reset(entityEditorState);
		}
		break;
		
		default:
		FLAT_ASSERT_MSG(false, "Unknown mode %u", mode);
	}
	
	getStateMachine().setState(std::move(state));
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
					case 'f':
					fullscreen = true;
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
		<< "\t-w\t\t\t\twindowed" << std::endl
		<< "\t-g <mod path> <map>\t\tgame mode" << std::endl
		<< "\t-m <mod path> <map>\t\tmap editor mode" << std::endl
		<< "\t-e <mod path> <map> <entity>\tentity editor mode" << std::endl;
	FLAT_BREAK();
	exit(1);
}

} // game



