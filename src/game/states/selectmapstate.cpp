#include "selectmapstate.h"
#include "../game.h"

namespace game
{
namespace states
{

void SelectMapState::enter(Game& game)
{
	if (game.modPath.empty())
	{
		game.modPath = "mods/crispy-guacamole";
	}

	Super::enter(game);

	game.assetRepository->addAssetDirectory(game.modPath);
	game.assetRepository->scanAllAssets();

	game.lua->doFile("data/selectmap/scripts/selectmap.lua");
}

} // states
} // game

