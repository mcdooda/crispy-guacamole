#ifndef GAME_MAP_TILETEMPLATE_H
#define GAME_MAP_TILETEMPLATE_H

#include <string>

#include "texturepack.h"
#include "navigability.h"

namespace game
{
class Game;
namespace map
{

class TileTemplate
{
	public:
		TileTemplate(const std::filesystem::path& path, Game& game);
		~TileTemplate();

		inline std::filesystem::path getName() const { return m_path.stem(); }

		inline const std::shared_ptr<const flat::video::Texture>& getTexture() const { return m_texture; }

		int getRandomTileVariantIndex(Game& game) const;
		int getRandomTileVariantIndex(Game& game, const std::vector<int>& tileVariantIndices) const;

		inline float getFrameDuration() const { return m_frameDuration; }

		inline int getNumFrames() const { return m_numFrames; }

		inline int getNumVariants() const { return static_cast<int>(m_tileVariantProbabilities.size()); }

		inline const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getSelectTile() const { return m_selectTile; }

		inline Navigability getNavigability() const { return m_navigability; }

	private:
		void loadTileConfig(Game& game, const std::filesystem::path& path);

	private:
		std::filesystem::path m_path;
		std::shared_ptr<const flat::video::Texture> m_texture;
		std::vector<float> m_tileVariantProbabilities;
		flat::lua::SharedLuaReference<LUA_TFUNCTION> m_selectTile;
		float m_tileVariantProbabilitiesSum;
		float m_frameDuration;
		int m_numFrames;
		Navigability m_navigability;
};

} // map
} // game

#endif // GAME_MAP_TILETEMPLATE_H