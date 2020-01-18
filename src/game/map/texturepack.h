#ifndef GAME_MAP_TEXTUREPACK_H
#define GAME_MAP_TEXTUREPACK_H

#include <flat.h>

namespace game
{
class Game;
namespace map
{

class TexturePack
{
public:
	TexturePack();

	void addTexture(const std::shared_ptr<const flat::video::Texture>& tileTexture, float probability);

	const std::shared_ptr<const flat::video::Texture>& getRandomTexture(Game& game) const;
	const std::shared_ptr<const flat::video::Texture>& getFirstTexture() const;

private:
	std::vector<std::pair<std::shared_ptr<const flat::video::Texture>, float>> m_textures;
	float m_probabilitiesSum;
};

} // map
} // game

#endif // GAME_MAP_TEXTUREPACK_H


