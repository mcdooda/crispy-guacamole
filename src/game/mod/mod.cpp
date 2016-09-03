#include <flat.h>
#include "mod.h"

namespace game
{
namespace mod
{

void Mod::setPath(const std::string& mapPath)
{
	m_mapPath = mapPath;
	if (m_mapPath[m_mapPath.size() - 1] != '/')
		m_mapPath += '/';

	m_path = m_mapPath + "../../";
}

void Mod::readConfig(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	FLAT_ASSERT_MSG(!m_path.empty(), "Mod path is empty");
	
	luaL_loadfile(L, getMapScriptPath("map.lua").c_str());
	lua_call(L, 0, 1);
	
	{
		lua_getfield(L, -1, "width");
		m_mapWidth = luaL_checkint(L, -1);
		lua_getfield(L, -2, "height");
		m_mapHeight = luaL_checkint(L, -1);
		lua_pop(L, 2);
	}

	{
		lua_getfield(L, -1, "axes");
		luaL_checktype(L, -1, LUA_TTABLE);
		{
			lua_getfield(L, -1, "x");
			luaL_checktype(L, -1, LUA_TTABLE);
			lua_rawgeti(L, -1, 1);
			m_xAxis.x = static_cast<float>(luaL_checknumber(L, -1));
			lua_rawgeti(L, -2, 2);
			m_xAxis.y = static_cast<float>(luaL_checknumber(L, -1));
			lua_pop(L, 3);
		}
		{
			lua_getfield(L, -1, "y");
			luaL_checktype(L, -1, LUA_TTABLE);
			lua_rawgeti(L, -1, 1);
			m_yAxis.x = static_cast<float>(luaL_checknumber(L, -1));
			lua_rawgeti(L, -2, 2);
			m_yAxis.y = static_cast<float>(luaL_checknumber(L, -1));
			lua_pop(L, 3);
		}
		{
			lua_getfield(L, -1, "z");
			luaL_checktype(L, -1, LUA_TTABLE);
			lua_rawgeti(L, -1, 1);
			m_zAxis.x = static_cast<float>(luaL_checknumber(L, -1));
			lua_rawgeti(L, -2, 2);
			m_zAxis.y = static_cast<float>(luaL_checknumber(L, -1));
			lua_pop(L, 3);
		}
		lua_pop(L, 1);
	}
	
	lua_pop(L, 1);
}

std::string Mod::getMapScriptPath(const std::string& fileName) const
{
	return m_mapPath + fileName;
}

std::string Mod::getModScriptPath(const std::string& fileName) const
{
	return m_path + "scripts/" + fileName;
}

std::string Mod::getTexturePath(const std::string& fileName) const
{
	return m_path + fileName;
}

std::string Mod::getMapPath() const
{
	return m_mapPath + "map.gpmap";
}

std::string Mod::getEntityTemplatePath(const std::string& entityTemplateName) const
{
	return m_path + "entities/" + entityTemplateName + "/"; // path to a directory
}

} // mod
} // game



