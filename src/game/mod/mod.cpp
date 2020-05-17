#include <flat.h>
#include "mod.h"

namespace game
{
namespace mod
{

void Mod::setPath(const std::filesystem::path& path)
{
	m_path = path;
}

std::filesystem::path Mod::getFilePath(const std::filesystem::path& fileName) const
{
	return m_path / fileName;
}

std::filesystem::path Mod::getScriptPath(const std::filesystem::path& fileName) const
{
	return m_path / "scripts" / fileName;
}

std::filesystem::path Mod::getTexturePath(const std::filesystem::path& fileName) const
{
	return m_path / fileName;
}

std::filesystem::path Mod::getTextureRelativePath(const std::filesystem::path& absolutePath) const
{
	return absolutePath.parent_path().stem() / absolutePath.filename();
}

std::filesystem::path Mod::getMapFilePath(const std::filesystem::path& mapName, const std::filesystem::path& fileName) const
{
	return m_path / "maps" / mapName / fileName;
}

std::filesystem::path Mod::getEntityTemplatePath(const std::filesystem::path& entityTemplateName) const
{
	return m_path / "entities" / entityTemplateName; // path to a directory
}

std::filesystem::path Mod::getTileTemplatePath(const std::filesystem::path& tileTemplateName) const
{
	return m_path / "tiles" / tileTemplateName; // path to a directory
}

std::filesystem::path Mod::getPropTemplatePath(const std::filesystem::path& propTemplateName) const
{
	return m_path / "props" / propTemplateName; // path to a directory
}

std::filesystem::path Mod::getFactionsConfigPath() const
{
	return m_path / "factions.lua";
}

std::filesystem::path Mod::getSamplePath(const std::filesystem::path& sampleName) const
{
	return m_path / "samples" / sampleName;
}

} // mod
} // game



