#include <iterator>
#include <flat.h>
#include "faction.h"
#include "../factionrelation.h"
#include "../../../game.h"
#include "../../../states/basemapstate.h"

namespace game
{
namespace entity
{
namespace faction
{
namespace lua
{

int open(lua_State* L, const std::filesystem::path& factionsConfigPath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	// load function
	luaL_loadfile(L, factionsConfigPath.string().c_str());

	// setup environment table
	static const luaL_Reg factionMethods[] = {
		{"faction",         l_faction},
		{"factionRelation", l_factionRelation}
	};

	static const flat::lua::table::KeyValuePair<int> factionRelations[] = {
		{"NEUTRAL",  FactionRelation::NEUTRAL},
		{"FRIENDLY", FactionRelation::FRIENDLY},
		{"HOSTILE",  FactionRelation::HOSTILE}
	};

	lua_createtable(L, 0, static_cast<int>(flat::size(factionMethods) + flat::size(factionRelations)));

	for (const luaL_Reg& reg : factionMethods)
	{
		lua_pushstring(L, reg.name);
		lua_pushcfunction(L, reg.func);
		lua_rawset(L, -3);
	}
	for (const flat::lua::table::KeyValuePair<int>& pair : factionRelations)
	{
		lua_pushstring(L, pair.key);
		lua_pushinteger(L, pair.value);
		lua_rawset(L, -3);
	}

	// set the table as the 1st upvalue
	lua_setupvalue(L, -2, 1);

	// call the function with the environment correctly set
	lua_call(L, 0, 0);

	return 0;
}

int l_faction(lua_State* L)
{
	const char* factionName = luaL_checkstring(L, 1);
	Game& game = flat::lua::getFlatAs<Game>(L);
	states::BaseMapState& baseMapState = game.getStateMachine().getState()->to<states::BaseMapState>();
	baseMapState.addFaction(factionName);
	return 0;
}

int l_factionRelation(lua_State* L)
{
	const char* faction1Name = luaL_checkstring(L, 1);
	FactionRelation factionRelation = static_cast<FactionRelation>(luaL_checkinteger(L, 2));
	const char* faction2Name = luaL_checkstring(L, 3);
	Game& game = flat::lua::getFlatAs<Game>(L);
	states::BaseMapState& baseMapState = game.getStateMachine().getState()->to<states::BaseMapState>();
	Faction* faction1 = baseMapState.getFactionByName(faction1Name);
	luaL_argcheck(L, faction1 != nullptr, 1, "Faction does not exist");
	Faction* faction2 = baseMapState.getFactionByName(faction2Name);
	luaL_argcheck(L, faction2 != nullptr, 3, "Faction does not exist");
	faction1->addFactionRelation(*faction2, factionRelation);
	return 0;
}

} // lua
} // faction
} // entity
} // game


