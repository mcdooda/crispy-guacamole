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

int open(lua_State* L, const std::string& factionsConfigPath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	static const luaL_Reg factionMethods[] = {
		{"faction",         l_faction},
		{"factionRelation", l_factionRelation}
	};
	for (const luaL_Reg& reg : factionMethods)
	{
		lua_pushcfunction(L, reg.func);
		lua_setglobal(L, reg.name);
	}

	static const flat::lua::table::KeyValuePair<int> factionRelations[] = {
		{"NEUTRAL",  FactionRelation::NEUTRAL},
		{"FRIENDLY", FactionRelation::FRIENDLY},
		{"HOSTILE",  FactionRelation::HOSTILE}
	};
	for (const flat::lua::table::KeyValuePair<int>& pair : factionRelations)
	{
		lua_pushinteger(L, pair.value);
		lua_setglobal(L, pair.key);
	}

	// TODO: load file with env instead of clearing global variables afterwards
	luaL_dofile(L, factionsConfigPath.c_str());

	const char* clearGlobals[] = {
		"faction",
		"factionRelation",
		"NEUTRAL",
		"FRIENDLY",
		"HOSTILE"
	};
	for (const char* name : clearGlobals)
	{
		lua_pushnil(L);
		lua_setglobal(L, name);
	}

	return 0;
}

int l_faction(lua_State* L)
{
	const char* factionName = luaL_checkstring(L, 1);
	Game& game = flat::lua::getGame(L).to<Game>();
	states::BaseMapState& baseMapState = game.getStateMachine().getState()->to<states::BaseMapState>();
	baseMapState.addFaction(factionName);
	return 0;
}

int l_factionRelation(lua_State* L)
{
	const char* faction1Name = luaL_checkstring(L, 1);
	FactionRelation factionRelation = static_cast<FactionRelation>(luaL_checkinteger(L, 2));
	const char* faction2Name = luaL_checkstring(L, 3);
	Game& game = flat::lua::getGame(L).to<Game>();
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


