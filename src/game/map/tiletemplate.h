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
		TileTemplate(Game& game, const std::string& path);
		~TileTemplate();

		inline const std::shared_ptr<const flat::video::Texture>& getTexture() const { return m_texture; }

		int getRandomTileVariantIndex(Game& game) const;

		inline float getFrameDuration() const { return m_frameDuration; }

		inline int getNumFrames() const { return m_numFrames; }
		inline int getNumVariants() const { return static_cast<int>(m_tileVariantProbabilities.size()); }

	private:
		void loadTileConfig(Game& game, const std::string& path);

	private:
		std::shared_ptr<const flat::video::Texture> m_texture;
		std::vector<float> m_tileVariantProbabilities;
		float m_tileVariantProbabilitiesSum;
		float m_frameDuration;
		int m_numFrames;
};
}
}


#endif // GAME_MAP_TILETEMPLATE_H


