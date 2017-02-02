#include "map.h"
#include "../../game.h"
#include "../../states/editorstate.h"
#include "../../entity/lua/entity.h"

namespace game
{
namespace map
{
namespace lua
{

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_createtable(L, 0, 3);
	static const luaL_Reg Map_lib_m[] = {
		{"getName",             l_Map_getName},
		{"load",                l_Map_load},
		{"save",                l_Map_save},

		{"getNumEntities",      l_Map_getNumEntities},

		{"getEntitiesInRange", l_Map_getEntitiesInRange},

		{nullptr, nullptr}
	};
	luaL_setfuncs(L, Map_lib_m, 0);
	lua_setglobal(L, "Map");
	
	return 0;
}

int l_Map_getName(lua_State * L)
{
	lua_pushstring(L, "map1");
	return 1;
}

int l_Map_load(lua_State* L)
{
	std::string mapName = luaL_checkstring(L, 1);
	Game& game = flat::lua::getGame(L).to<Game>();
	flat::state::State* state = game.getStateMachine().getState();
	states::BaseMapState& baseMapState = state->as<states::BaseMapState>();
	bool mapLoaded = baseMapState.loadMap(game, mapName);
	lua_pushboolean(L, mapLoaded);
	return 1;
}

int l_Map_save(lua_State* L)
{
	Game& game = flat::lua::getGame(L).to<Game>();
	flat::state::State* state = game.getStateMachine().getState();
	states::EditorState& editorState = state->as<states::EditorState>();
	bool mapSaved = editorState.saveMap(game);
	lua_pushboolean(L, mapSaved);
	return 1;
}

int l_Map_getNumEntities(lua_State* L)
{
	Game& game = flat::lua::getGame(L).to<Game>();
	flat::state::State* state = game.getStateMachine().getState();
	states::BaseMapState& baseMapState = state->as<states::BaseMapState>();
	lua_pushinteger(L, baseMapState.getMap().getEntities().size());
	return 1;
}

int l_Map_getEntitiesInRange(lua_State* L)
{
	float x = static_cast<float>(luaL_checknumber(L, 1));
	float y = static_cast<float>(luaL_checknumber(L, 2));
	float range = static_cast<float>(luaL_checknumber(L, 3));
	Game& game = flat::lua::getGame(L).to<Game>();
	flat::state::State* state = game.getStateMachine().getState();
	states::BaseMapState& baseMapState = state->as<states::BaseMapState>();
	const map::Map& map = baseMapState.getMap();
	lua_newtable(L);
	int i = 1;
	map.eachEntityInRange(flat::Vector2(x, y), range, [L, &i](entity::Entity* entity)
	{
		entity::lua::pushEntity(L, entity);
		lua_rawseti(L, -2, i);
		++i;
	});
	return 1;
}

} // lua
} // map
} // game