#include "selectmapstate.h"
#include "game.h"

namespace game::states
{

void SelectMapState::enter(Game& game)
{
	Super::enter(game);
	game.rescanAssets();
	game.lua->doFile("data/selectmap/scripts/selectmap.lua");
}

} // game::states
