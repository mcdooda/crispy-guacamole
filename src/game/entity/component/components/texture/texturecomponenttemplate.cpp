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
	m_origin = flat::lua::getVector2(L, -1);

	lua_pop(L, 1);
}

} // texture
} // component
} // entity
} // game


