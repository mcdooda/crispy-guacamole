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
			if (mod.getPath().empty() || mapPath.empty())
			{
				wrongArguments();
			}
			states::GameState* gameState = new states::GameState();
			state.reset(gameState);
		}
		break;
		
		case Mode::MAPEDITOR:
		{
			if (mod.getPath().empty() || mapPath.empty())
			{
				wrongArguments();
			}
			states::MapEditorState* mapEditorState = new states::MapEditorState();
			state.reset(mapEditorState);
		}
		break;

		case Mode::ENTITYEDITOR:
		{
			if (mod.getPath().empty() || mapPath.empty() || entityPath.empty())
			{
				wrongArguments();
			}
			states::EntityEditorState* entityEditorState = new states::EntityEditorState();
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
					mod.setPath(argGetString(++i));
					mapPath = argGetString(++i);
					break;
					
					case 'm':
					mode = Mode::MAPEDITOR;
					mod.setPath(argGetString(++i));
					mapPath = argGetString(++i);
					break;

					case 'e':
					mode = Mode::ENTITYEDITOR;
					mod.setPath(argGetString(++i));
					mapPath = argGetString(++i);
					entityPath = argGetString(++i);
					break;

					case 'o':
					mod.setPath(argGetString(++i));
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

	if (mod.getPath().empty())
	{
		mod.setPath("mods/crispy-guacamole");
	}
}

void Game::scanAssets()
{
	assetRepository->addAssetDirectory(mod.getPath());
	Super::scanAssets();
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



