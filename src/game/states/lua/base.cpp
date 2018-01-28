#include "game.h"
#include "../basestate.h"
#include "../../game.h"

namespace game
{
namespace states
{
namespace lua
{
namespace base
{

BaseState& getBaseState(lua_State* L)
{
	Game& game = flat::lua::getFlatAs<Game>(L);
	return game.getStateMachine().getState()->to<BaseState>();
}

} // base
} // lua
} // states
} // game