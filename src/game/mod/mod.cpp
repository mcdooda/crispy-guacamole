#include <flat.h>
#include "mod.h"

namespace game
{
namespace mod
{

void Mod::setPath(const std::string& path)
{
	m_path = path;
	if (m_path[m_path.size() - 1] != '/')
		m_path += '/';
}

std::string Mod::getScriptPath(const std::string& fileName) const
{
	return m_path + "scripts/" + fileName;
}

std::string Mod::getTexturePath(const std::string& fileName) const
{
	return m_path + fileName;
}

std::string Mod::getTextureRelativePath(const std::string& absolutePath) const
{
	FLAT_ASSERT(std::strncmp(m_path.c_str(), absolutePath.c_str(), m_path.size()) == 0);
	return absolutePath.substr(m_path.size(), absolutePath.size() - m_path.size());
}

std::string Mod::getMapPath(const std::string& mapName, const char* fileName) const
{
	return m_path + "maps/" + mapName + "/" + fileName;
}

std::string Mod::getEntityTemplatePath(const std::string& entityTemplateName) const
{
	return m_path + "entities/" + entityTemplateName + "/"; // path to a directory
}

std::string Mod::getTileTemplatePath(const std::string& tileTemplateName) const
{
	return m_path + "tiles/" + tileTemplateName + "/"; // path to a directory
}

std::string Mod::getPropTemplatePath(const std::string& propTemplateName) const
{
	return m_path + "props/" + propTemplateName + "/"; // path to a directory
}

} // mod
} // game



