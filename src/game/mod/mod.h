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
		void setPath(const std::filesystem::path& path);
		inline const std::filesystem::path& getPath() const { return m_path; }

		std::filesystem::path getFilePath(const std::filesystem::path& fileName) const;

		std::filesystem::path getScriptPath(const std::filesystem::path& fileName) const;
		std::filesystem::path getTexturePath(const std::filesystem::path& fileName) const;
		std::filesystem::path getTextureRelativePath(const std::filesystem::path& absolutePath) const;
		std::filesystem::path getTileTemplatePath(const std::filesystem::path& tileTemplateName) const;
		std::filesystem::path getPropTemplatePath(const std::filesystem::path& propTemplateName) const;
		std::filesystem::path getFactionsConfigPath() const;
		std::filesystem::path getSamplePath(const std::filesystem::path& sampleName) const;

	private:
		std::filesystem::path m_path;
};

} // mod
} // game

#endif // GAME_MOD_MOD_H



