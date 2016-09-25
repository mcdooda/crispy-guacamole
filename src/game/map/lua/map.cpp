#include "map.h"
#include "../../game.h"
#include "../../states/editorstate.h"

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
		{"getName", l_Map_getName},
		{"load",    l_Map_load},
		{"save",    l_Map_save},
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
	Game* game = flat::lua::getGameAs<Game>(L);
	flat::state::State* state = game->getStateMachine()->getCurrentState();
	FLAT_ASSERT(dynamic_cast<states::BaseMapState*>(state) != nullptr);
	states::BaseMapState* baseMapState = static_cast<states::BaseMapState*>(state);
	bool mapLoaded = baseMapState->loadMap(game, mapName);
	lua_pushboolean(L, mapLoaded);
	return 1;
}

int l_Map_save(lua_State* L)
{
	Game* game = flat::lua::getGameAs<Game>(L);
	flat::state::State* state = game->getStateMachine()->getCurrentState();
	FLAT_ASSERT(dynamic_cast<states::EditorState*>(state) != nullptr);
	states::EditorState* editorState = static_cast<states::EditorState*>(state);
	bool mapSaved = editorState->saveMap(game);
	lua_pushboolean(L, mapSaved);
	return 1;
}

}
}
}