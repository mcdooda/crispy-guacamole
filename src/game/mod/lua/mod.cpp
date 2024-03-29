#include "mod.h"
#include "../../game.h"
#include "../../states/selectmapstate.h"

namespace game
{
namespace mod
{
namespace lua
{

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_createtable(L, 0, 1);
	static const luaL_Reg Mod_lib_m[] = {
		{"getPath",             l_Mod_getPath},
		{"getCurrentMapPath",   l_Mod_getCurrentMapPath},

		{"getEntities",         l_Mod_getEntities},
		{"getMaps",             l_Mod_getMaps},
		{"getProps",            l_Mod_getProps},
		{"getTiles",            l_Mod_getTiles},

		{"openEditor",          l_Mod_openEditor},

		{"getFilePath",         l_Mod_getFilePath},
		{"getRelativeFilePath", l_Mod_getRelativeFilePath},

		{nullptr, nullptr}
	};
	luaL_setfuncs(L, Mod_lib_m, 0);
	lua_setglobal(L, "Mod");

	return 0;
}

int l_Mod_getPath(lua_State* L)
{
	Game& game = flat::lua::getFlatAs<Game>(L);
	std::string path = game.mod.getPath().string();
	flat::lua::formatPathToLua(path);
	lua_pushstring(L, path.c_str());
	return 1;
}

int l_Mod_getCurrentMapPath(lua_State* L)
{
	Game& game = flat::lua::getFlatAs<Game>(L);
	lua_pushstring(L, game.mapPath.string().c_str());
	return 1;
}

namespace
{
static int locGetAssetsTable(lua_State* L, const char* assetDirectory)
{
	Game& game = flat::lua::getFlatAs<Game>(L);

	std::shared_ptr<flat::file::Directory> directory = flat::file::Directory::open(game.mod.getPath() / assetDirectory);

	lua_createtable(L, 4, 0);
	int i = 1;
	directory->eachSubFile(
		[L, &i](const std::shared_ptr<flat::file::File>& file)
		{
			if (file->is<flat::file::Directory>())
			{
				std::string directoryName = file->getFileName();
				lua_pushstring(L, directoryName.c_str());
				lua_rawseti(L, -2, i);
				++i;
			}
		}
	);

	return 1;
}
}

int l_Mod_getEntities(lua_State* L)
{
	return locGetAssetsTable(L, "entities");
}

int l_Mod_getMaps(lua_State* L)
{
	return locGetAssetsTable(L, "maps");
}

int l_Mod_getProps(lua_State* L)
{
	return locGetAssetsTable(L, "props");
}

int l_Mod_getTiles(lua_State* L)
{
	return locGetAssetsTable(L, "tiles");
}


int l_Mod_openEditor(lua_State* L)
{
	const char* modPath = luaL_checkstring(L, 1);
	Game& game = flat::lua::getFlatAs<Game>(L);
	game.mod.setPath(modPath);
	game.mapPath = "";
	game.entityPath = "";
	std::unique_ptr<states::SelectMapState> selectMapState = std::make_unique<states::SelectMapState>();
	game.getStateMachine().setNextState(std::move(selectMapState));
	return 0;
}

int l_Mod_getFilePath(lua_State* L)
{
	const char* fileName = luaL_checkstring(L, 1);
	Game& game = flat::lua::getFlatAs<Game>(L);
	std::string path = game.mod.getFilePath(fileName).string();
	flat::lua::formatPathToLua(path);
	lua_pushstring(L, path.c_str());
	return 1;
}

int l_Mod_getRelativeFilePath(lua_State* L)
{
	std::string path = luaL_checkstring(L, 1);
	flat::lua::formatPathFromLua(path);
	Game& game = flat::lua::getFlatAs<Game>(L);
	std::string relativePath = game.mod.getRelativePath(path).string();
	flat::lua::formatPathToLua(relativePath);
	lua_pushstring(L, relativePath.c_str());
	return 1;
}

} // lua
} // mod
} // game


