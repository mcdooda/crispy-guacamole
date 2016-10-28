#include "texturecomponenttemplate.h"
#include "../../../../game.h"

namespace game
{
namespace entity
{
namespace component
{
namespace texture
{

void TextureComponentTemplate::load(Game& game, lua_State* L, const std::string& entityTemplatePath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	m_texture = game.video->getTexture(entityTemplatePath + "texture.png");

	lua_getfield(L, -1, "origin");
	lua_rawgeti(L, -1, 1);
	m_origin.x = static_cast<float>(luaL_checknumber(L, -1));
	lua_rawgeti(L, -2, 2);
	m_origin.y = static_cast<float>(luaL_checknumber(L, -1));

	lua_pop(L, 3);
}

} // texture
} // component
} // entity
} // game


