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
		PropTemplate(const std::string& path, Game& game);
		~PropTemplate();

		inline std::shared_ptr<const flat::video::Texture> getRandomTexture(Game& game) const { return m_texturePack.getRandomTexture(game); }
		inline std::shared_ptr<const flat::video::Texture> getFirstTexture() const { return m_texturePack.getFirstTexture(); }

	private:
		void loadPropConfig(Game& game, const std::string& path);

	private:
		TexturePack m_texturePack;
};
}
}


#endif // GAME_MAP_PROPTEMPLATE_H


