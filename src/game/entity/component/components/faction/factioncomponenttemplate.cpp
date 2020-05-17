#include <flat.h>
#include "factioncomponenttemplate.h"
#include "../../../../game.h"
#include "../../../../states/basemapstate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace faction
{

void FactionComponentTemplate::load(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_getfield(L, -1, "faction");
	const char* factionName = luaL_checkstring(L, -1);

	const states::BaseMapState& baseMapState = game.getStateMachine().getState()->as<states::BaseMapState>();
	m_faction = baseMapState.getFactionByName(factionName);
	luaL_argcheck(L, m_faction != nullptr, 0, "Faction does not exist");

	lua_pop(L, 1);
}

} // faction
} // component
} // entity
} // game


