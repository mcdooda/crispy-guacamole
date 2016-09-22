#ifndef GAME_STATES_EDITOR_TEXTUREPACK_TEXTUREPACK_H
#define GAME_STATES_EDITOR_TEXTUREPACK_TEXTUREPACK_H

#include <flat.h>

namespace game
{
class Game;
namespace states
{
namespace editor
{
namespace texturepack
{

	class TexturePack
	{
	public:
		TexturePack();
		TexturePack(TexturePack&& other);
		void operator=(TexturePack&& other);

		void addTexture(const std::shared_ptr<const flat::video::Texture>& tileTexture, float probability);

		const std::shared_ptr<const flat::video::Texture>& getRandomTexture(Game* game) const;

	private:
		std::vector<std::pair<std::shared_ptr<const flat::video::Texture>, float>> m_textures;
		float m_probabilitiesSum;
	};

} // texturepack
} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_TEXTUREPACK_TEXTUREPACK_H


