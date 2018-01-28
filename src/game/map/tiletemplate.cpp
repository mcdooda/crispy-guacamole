#include <flat.h>
#include "tiletemplate.h"
#include "../game.h"

namespace game
{
namespace map
{

TileTemplate::TileTemplate(Game& game, const std::string& path)
{
	loadTileConfig(game, path);
}

TileTemplate::~TileTemplate()
{

}

void TileTemplate::loadTileConfig(Game& game, const std::string& path)
{
	lua_State* L = game.lua->getMainState();
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		std::string tileConfigPath = path + "tile.lua";
		luaL_loadfile(L, tileConfigPath.c_str());
		lua_call(L, 0, 1);

		luaL_checktype(L, -1, LUA_TTABLE);

		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			const char* tileName = luaL_checkstring(L, -2);
			float probability = static_cast<float>(luaL_checknumber(L, -1));

			std::string tileItemPath = tileConfigPath = path + tileName + ".png";
			std::shared_ptr<const flat::video::FileTexture> tileTexture = game.video->getTexture(tileConfigPath);
			m_texturePack.addTexture(tileTexture, probability);

			lua_pop(L, 1);
		}

		lua_pop(L, 1);
	}
}

} // map
} // game


