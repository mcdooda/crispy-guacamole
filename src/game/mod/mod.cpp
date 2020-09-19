#include <flat.h>
#include "mod.h"

namespace game
{
namespace mod
{

void Mod::setPath(const std::filesystem::path& path)
{
	m_path = path;
	m_path.make_preferred();
}

std::filesystem::path Mod::getFilePath(const std::filesystem::path& fileName) const
{
	return m_path / fileName;
}

std::filesystem::path Mod::getRelativePath(const std::filesystem::path& path) const
{
	return path.lexically_relative(m_path);
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
	// TODO fix using the appropriate path function
	return absolutePath.parent_path().stem() / absolutePath.filename();
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



