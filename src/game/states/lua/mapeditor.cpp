#include "mapeditor.h"
#include "../mapeditorstate.h"
#include "../../game.h"

namespace game
{
namespace states
{
namespace lua
{
namespace mapeditor
{

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_createtable(L, 0, 1);
	static const luaL_Reg MapEditor_lib_f[] = {
		{"openMap", l_MapEditor_openMap},

		{nullptr, nullptr}
	};
	luaL_setfuncs(L, MapEditor_lib_f, 0);
	lua_setglobal(L, "MapEditor");

	return 0;
}

int l_MapEditor_openMap(lua_State* L)
{
	const char* modPath = luaL_checkstring(L, 1);
	const char* mapName = luaL_checkstring(L, 2);
	Game& game = flat::lua::getFlatAs<Game>(L);
	game.mod.setPath(modPath);
	game.mapName = mapName;
	std::unique_ptr<MapEditorState> mapEditorState = std::make_unique<MapEditorState>();
	game.getStateMachine().setNextState(std::move(mapEditorState));
	return 1;
}

} // editor
} // lua
} // states
} // game