#include <flat.h>
#include "mod.h"

namespace game
{
namespace mod
{

void Mod::readConfig(lua_State* L)
{
	FLAT_ASSERT_MSG(!m_path.empty(), "Mod path is empty");
	FLAT_ASSERT(L);
	
	int top = lua_gettop(L);
	
	luaL_loadfile(L, getScriptPath("config.lua").c_str());
	lua_call(L, 0, 1);
	
	lua_getfield(L, -1, "mapWidth");
	m_mapWidth = luaL_checkint(L, -1);
	
	lua_getfield(L, -2, "mapHeight");
	m_mapHeight = luaL_checkint(L, -1);
	
	lua_getfield(L, -3, "defaultTile");
	m_defaultTile = luaL_checkstring(L, -1);
	
	lua_getfield(L, -4, "forestDoodad");
	m_forestDoodad = luaL_checkstring(L, -1);
	
	lua_getfield(L, -5, "resourcePath");
	if (lua_isstring(L, -1))
		m_resourcePath = luaL_checkstring(L, -1);

	lua_settop(L, top);
}

std::string Mod::getScriptPath(const std::string& fileName) const
{
	return m_path + "/scripts/" + fileName;
}

std::string Mod::getTexturePath(const std::string& fileName) const
{
	return m_resourcePath + "/" + fileName;
}

std::string Mod::getMapPath() const
{
	return m_path + "/map.gpmap";
}

} // mod
} // game



