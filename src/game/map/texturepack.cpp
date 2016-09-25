#include "texturepack.h"
#include "../game.h"

namespace game
{
namespace map
{

TexturePack::TexturePack() :
	m_probabilitiesSum(0.f)
{
}

void TexturePack::addTexture(const std::shared_ptr<const flat::video::Texture>& tileTexture, float probability)
{
	std::pair<std::shared_ptr<const flat::video::Texture>, float> pair;
	pair.first = tileTexture;
	pair.second = probability;
	auto pred = [&tileTexture](const std::pair<std::shared_ptr<const flat::video::Texture>, float>& pair) { return pair.first == tileTexture; };
	if (std::find_if(m_textures.begin(), m_textures.end(), pred) == m_textures.end())
	{
		m_textures.push_back(pair);
		m_probabilitiesSum += probability;
	}
}

const std::shared_ptr<const flat::video::Texture>& TexturePack::getRandomTexture(Game* game) const
{
	FLAT_ASSERT(!m_textures.empty() && m_probabilitiesSum > 0.f);
	float random = game->random->nextFloat(0.f, m_probabilitiesSum);
	int randomIndex = 0;
	for (const std::pair<std::shared_ptr<const flat::video::Texture>, float>& pair : m_textures)
	{
		if (random <= pair.second)
		{
			break;
		}
		random -= pair.second;
		++randomIndex;
	}
	return m_textures.at(randomIndex).first;
}

} // map
} // game


