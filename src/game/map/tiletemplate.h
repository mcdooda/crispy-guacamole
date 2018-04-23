#ifndef GAME_MAP_TILETEMPLATE_H
#define GAME_MAP_TILETEMPLATE_H

#include <string>
#include "texturepack.h"

namespace game
{
class Game;
namespace map
{
class TileTemplate
{
	public:
		TileTemplate(Game& game, const std::string& name);
		~TileTemplate();

		inline const std::string& getName() const { return m_name; }

		inline const std::shared_ptr<const flat::video::Texture>& getTexture() const { return m_texture; }

		int getRandomTileVariantIndex(Game& game) const;
		int getRandomTileVariantIndex(Game& game, const std::vector<int>& tileVariantIndices) const;

		inline float getFrameDuration() const { return m_frameDuration; }

		inline int getNumFrames() const { return m_numFrames; }

		inline int getNumVariants() const { return static_cast<int>(m_tileVariantProbabilities.size()); }

		inline const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getSelectTile() const { return m_selectTile; }

	private:
		void loadTileConfig(Game& game, const std::string& path);

	private:
		std::string m_name;
		std::shared_ptr<const flat::video::Texture> m_texture;
		std::vector<float> m_tileVariantProbabilities;
		flat::lua::SharedLuaReference<LUA_TFUNCTION> m_selectTile;
		float m_tileVariantProbabilitiesSum;
		float m_frameDuration;
		int m_numFrames;
};
}
}


#endif // GAME_MAP_TILETEMPLATE_H


