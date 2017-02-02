#ifndef GAME_MAP_PROPTEMPLATE_H
#define GAME_MAP_PROPTEMPLATE_H

#include <string>
#include "texturepack.h"

namespace game
{
class Game;
namespace map
{
class PropTemplate
{
	public:
		PropTemplate(Game& game, const std::string& path);
		~PropTemplate();

		inline std::shared_ptr<const flat::video::Texture> getRandomTexture(Game& game) const { return m_texturePack.getRandomTexture(game); }

	private:
		void loadPropConfig(Game& game, const std::string& path);

	private:
		TexturePack m_texturePack;
};
}
}


#endif // GAME_MAP_PROPTEMPLATE_H


