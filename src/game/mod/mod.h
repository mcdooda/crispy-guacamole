#ifndef GAME_MOD_MOD_H
#define GAME_MOD_MOD_H

#include <iostream>
#include <flat.h>

namespace game
{
namespace mod
{

class Mod final
{
	public:
		void setPath(const std::string& path);
		inline const std::string& getPath() const { return m_path; }
		
		std::string getScriptPath(const std::string& fileName) const;
		std::string getTexturePath(const std::string& fileName) const;
		std::string getTextureRelativePath(const std::string& absolutePath) const;
		std::string getMapPath(const std::string& mapName, const char* fileName = "map.gpmap") const;
		std::string getEntityTemplatePath(const std::string& entityTemplateName) const;
		std::string getTileTemplatePath(const std::string& tileTemplateName) const;
		std::string getPropTemplatePath(const std::string& propTemplateName) const;
		std::string getFactionsConfigPath() const;
		
	private:
		std::string m_path;
};

} // mod
} // game

#endif // GAME_MOD_MOD_H



