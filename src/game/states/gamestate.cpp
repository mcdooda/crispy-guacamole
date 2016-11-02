#include "gamestate.h"
#include "../game.h"
#include "../entity/entity.h"
#include "../entity/entitytemplate.h"

namespace game
{
namespace states
{

void GameState::enter(Game& game)
{
	Super::enter(game);
	
	setCameraZoom(2.f);
	
	flat::lua::doFile(m_luaState, "data/game/scripts/ui.lua");
}

void GameState::execute(Game& game)
{
	despawnEntities();
	flat::time::Time* time = game.time;
	m_map.updateEntities(time->getTime(), time->getFrameTime());
	
	Super::execute(game);
}

} // states
} // game



