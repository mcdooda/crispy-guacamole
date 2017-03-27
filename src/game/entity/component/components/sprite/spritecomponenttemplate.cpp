#include "spritecomponenttemplate.h"
#include "../../../../game.h"

namespace game
{
namespace entity
{
namespace component
{
namespace sprite
{

void SpriteComponentTemplate::load(Game& game, lua_State* L, const std::string& entityTemplatePath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	std::shared_ptr<const flat::video::Texture> atlas = game.video->getTexture(entityTemplatePath + "atlas.png");
	m_spriteDescription.setAtlas(atlas);

	// atlas size
	lua_getfield(L, -1, "size");
	lua_rawgeti(L, -1, 1);
	int atlasWidth = static_cast<int>(luaL_checkinteger(L, -1));
	m_spriteDescription.setAtlasWidth(atlasWidth);

	lua_rawgeti(L, -2, 2);
	int atlasHeight = static_cast<int>(luaL_checkinteger(L, -1));
	m_spriteDescription.setAtlasHeight(atlasHeight);

	lua_pop(L, 3);

	// sprite origin
	flat::Vector2 origin;
	lua_getfield(L, -1, "origin");
	lua_rawgeti(L, -1, 1);
	origin.x = static_cast<float>(luaL_checknumber(L, -1));
	lua_rawgeti(L, -2, 2);
	origin.y = static_cast<float>(luaL_checknumber(L, -1));
	m_spriteDescription.setOrigin(origin);

	lua_pop(L, 3);

	// animations
	lua_getfield(L, -1, "animations");
	luaL_checktype(L, -1, LUA_TTABLE);

	lua_pushnil(L);
	while (lua_next(L, -2) != 0)
	{
		const char* animationName = luaL_checkstring(L, -2);

		luaL_checktype(L, -1, LUA_TTABLE);
		lua_getfield(L, -1, "line");
		int line = static_cast<int>(luaL_checkinteger(L, -1));
		if (line > atlasHeight)
		{
			luaL_error(L, "Animation %s, line = %d is out of the atlas", animationName, line);
		}
		lua_getfield(L, -2, "numFrames");
		int numFrames = static_cast<int>(luaL_checkinteger(L, -1));
		if (numFrames > atlasWidth)
		{
			luaL_error(L, "Animation %s, numFrames = %d is out of the atlas", animationName, numFrames);
		}
		lua_getfield(L, -3, "frameDuration");
		float frameDuration = static_cast<float>(luaL_checknumber(L, -1));

		sprite::AnimationDescription animationDescription(line - 1, numFrames, frameDuration);
		m_spriteDescription.addAnimationDescription(animationName, animationDescription);

		lua_pop(L, 4);
	}
	m_spriteDescription.cacheMandatoryAnimationDescriptions();

	lua_pop(L, 1);

	// attach points
	lua_getfield(L, -1, "attachPoints");
	if (!lua_isnil(L, -1))
	{
		luaL_checktype(L, -1, LUA_TTABLE);

		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			const char* attachPointName = luaL_checkstring(L, -2);

			luaL_checktype(L, -1, LUA_TTABLE);

			lua_rawgeti(L, -1, 1);
			float x = static_cast<float>(luaL_checknumber(L, -1));

			lua_rawgeti(L, -2, 2);
			float y = static_cast<float>(luaL_checknumber(L, -1));

			m_spriteDescription.addAttachPoint(attachPointName, flat::Vector2(x, y));

			lua_pop(L, 3);
		}
	}

	lua_pop(L, 1);
}

} // sprite
} // component
} // entity
} // game


