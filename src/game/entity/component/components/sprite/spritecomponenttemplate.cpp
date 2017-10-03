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
	flat::Vector2 size = flat::lua::getVector2(L, -1);
	int atlasWidth = static_cast<int>(size.x);
	int atlasHeight = static_cast<int>(size.y);
	m_spriteDescription.setAtlasWidth(atlasWidth);
	m_spriteDescription.setAtlasHeight(atlasHeight);
	lua_pop(L, 1);

	// sprite origin
	lua_getfield(L, -1, "origin");
	flat::Vector2 origin = flat::lua::getVector2(L, -1);
	m_spriteDescription.setOrigin(origin);
	lua_pop(L, 1);

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
			flat::Vector2 attachPointPosition = flat::lua::getVector2(L, -1);

			m_spriteDescription.addAttachPoint(attachPointName, attachPointPosition);

			lua_pop(L, 1);
		}
	}

	lua_pop(L, 1);
}

} // sprite
} // component
} // entity
} // game


