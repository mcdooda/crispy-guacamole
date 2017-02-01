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

		inline std::shared_ptr<const flat::video::Texture> getRandomTexture(Game& game) const { return m_texturePack.getRandomTexture(game); }

	private:
		void loadTileConfig(Game& game, const std::string& path);

	private:
		TexturePack m_texturePack;
};
}
}


#endif // GAME_MAP_TILETEMPLATE_H


