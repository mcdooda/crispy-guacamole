#include "../game.h"
#include "globalstate.h"

namespace game
{
namespace states
{

void GlobalState::enter(flat::state::Agent* agent)
{
	game::Game* game = agent->to<game::Game>();
	game->video->window->setTitle("Crispy guacamole");

	resetViews(game);
}

void GlobalState::execute(flat::state::Agent* agent)
{
	game::Game* game = (game::Game*) agent;
	
	if (game->input->window->isResized())
		resetViews(game);
		
	if (game->input->keyboard->isJustPressed(K(ESCAPE)))
		game->stop();
}

void GlobalState::exit(flat::state::Agent* agent)
{
	
}

void GlobalState::resetViews(game::Game* game)
{
	const flat::geometry::Vector2 windowSize = game->video->window->getSize();
	
	game->interfaceView.reset();
	game->interfaceView.move(windowSize / -2.0f);
	game->interfaceView.updateProjection(windowSize);
}

} // states
} // game


