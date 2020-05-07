#include "selectmapstate.h"
#include "../game.h"

namespace game
{
namespace states
{

void SelectMapState::enter(Game& game)
{
	if (game.mod.getPath().empty())
	{
		game.mod.setPath("mods/crispy-guacamole");
	}

	Super::enter(game);

	game.assetRepository->addAssetDirectory(game.mod.getPath());
	game.assetRepository->scanAllAssets();

	game.lua->doFile("data/selectmap/scripts/selectmap.lua");
}

} // states
} // game

