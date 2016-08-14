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
		void setPath(const std::string& path);
		inline const std::string& getPath() const { return m_path; }
		
		void readConfig(lua_State* L);
		
		std::string getScriptPath(const std::string& fileName) const;
		std::string getTexturePath(const std::string& fileName) const;
		std::string getMapPath() const;
		std::string getEntityTemplatePath(const std::string& entityTemplateName) const;
		
	private:
		std::string m_path;
		std::string m_resourcePath;
		
		// config
		int m_mapWidth;
		int m_mapHeight;
};

} // mod
} // game

#endif // GAME_MOD_MOD_H



