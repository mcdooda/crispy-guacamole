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
		void setPath(const std::string& mapPath);
		inline const std::string& getPath() const { return m_path; }
		
		void readConfig(lua_State* L);

		inline int getMapWidth() const { return m_mapWidth; }
		inline int getMapHeight() const { return m_mapHeight; }

		inline const flat::geometry::Vector2& getXAxis() const { return m_xAxis; }
		inline const flat::geometry::Vector2& getYAxis() const { return m_yAxis; }
		inline const flat::geometry::Vector2& getZAxis() const { return m_zAxis; }
		
		std::string getMapScriptPath(const std::string& fileName) const;
		std::string getModScriptPath(const std::string& fileName) const;
		std::string getTexturePath(const std::string& fileName) const;
		std::string getMapPath() const;
		std::string getEntityTemplatePath(const std::string& entityTemplateName) const;
		
	private:
		std::string m_path;
		std::string m_mapPath;
		
		// config
		int m_mapWidth;
		int m_mapHeight;

		flat::geometry::Vector2 m_xAxis;
		flat::geometry::Vector2 m_yAxis;
		flat::geometry::Vector2 m_zAxis;
};

} // mod
} // game

#endif // GAME_MOD_MOD_H



