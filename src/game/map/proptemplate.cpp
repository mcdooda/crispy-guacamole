#include <flat.h>
#include "proptemplate.h"
#include "../game.h"

namespace game
{
namespace map
{

PropTemplate::PropTemplate(const std::filesystem::path& path, Game& game)
{
	loadPropConfig(game, path);
}

PropTemplate::~PropTemplate()
{

}

void PropTemplate::loadPropConfig(Game& game, const std::filesystem::path& path)
{
	lua_State* L = game.lua->state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		const std::filesystem::path propConfigPath = path / "prop.lua";
		luaL_loadfile(L, propConfigPath.string().c_str());
		lua_call(L, 0, 1);

		luaL_checktype(L, -1, LUA_TTABLE);

		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			const char* propName = luaL_checkstring(L, -2);
			float probability = static_cast<float>(luaL_checknumber(L, -1));

			const std::filesystem::path propItemPath = path / (std::string(propName) + ".png");
			std::shared_ptr<const flat::video::FileTexture> propTexture = game.video->getTexture(propItemPath);
			m_texturePack.addTexture(propTexture, probability);

			lua_pop(L, 1);
		}

		lua_pop(L, 1);
	}
}

} // map
} // game


