#ifndef GAME_MOD_MOD_H
#define GAME_MOD_MOD_H

#include <iostream>
#include <lua5.2/lua.hpp>

namespace game
{
namespace mod
{

class Mod final
{
	public:
		inline void setPath(const std::string& path) { m_path = path; }
		void readConfig(lua_State* L);
		
		std::string getScriptPath(const std::string& fileName) const;
		std::string getTexturePath(const std::string& fileName) const;
		std::string getMapPath() const;
		
	private:
		std::string m_path;
		std::string m_resourcePath;
		
		// config
		int m_mapWidth;
		int m_mapHeight;
		
		std::string m_defaultTile;
		std::string m_forestDoodad;
};

} // mod
} // game

#endif // GAME_MOD_MOD_H



