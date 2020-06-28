#include "propcomponenttemplate.h"
#include "game.h"

namespace game
{
namespace entity
{
namespace component
{
namespace prop
{

void PropComponentTemplate::load(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath)
{
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		// prop size
		lua_getfield(L, -1, "size");
		m_size = flat::lua::getVector2(L, -1);

		// navigability
		lua_getfield(L, -2, "navigability");
		m_navigability = static_cast<map::Navigability>(luaL_checkinteger(L, -1));

		lua_pop(L, 2);
	}

	// load lua animation selector file
	std::filesystem::path selectAnimationPath = entityTemplatePath / "prop_selectanimation.lua";
	if (std::filesystem::exists(selectAnimationPath))
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
		flat::lua::loadFile(L, selectAnimationPath);
		if (!lua_isnil(L, -1))
		{
			luaL_checktype(L, -1, LUA_TFUNCTION);
			lua_pcall(L, 0, 1, 0);
			m_selectAnimation.setIfNotNil(L, -1);
		}
		lua_pop(L, 1);
	}
}

} // prop
} // component
} // entity
} // game


