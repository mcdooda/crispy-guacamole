#include "tile.h"
#include "map.h"

namespace game
{
namespace map
{

Tile::Tile() :
	m_exists(true),
	m_doodadSprite(nullptr)
{
	m_sprite.setColor(flat::video::Color::WHITE);
}

Tile::~Tile()
{
	FLAT_DELETE(m_doodadSprite);
}

void Tile::draw(const flat::util::RenderSettings& renderSettings, const flat::geometry::Matrix4& viewMatrix) const
{
	FLAT_ASSERT(m_exists);
	m_sprite.draw(renderSettings, viewMatrix);
	if (m_doodadSprite)
	{
		m_doodadSprite->draw(renderSettings, viewMatrix);
	}
}

void Tile::setCoordinates(const Map& map, int x, int y, float z)
{
	FLAT_ASSERT(m_exists);
	m_x = x;
	m_y = y;
	m_z = z;
	computeDepth(x, y, -0.5f);
	
	const flat::geometry::Vector2 xAxis = map.getXAxis();
	const flat::geometry::Vector2 yAxis = map.getYAxis();
	const flat::geometry::Vector2 zAxis = map.getZAxis();
	
	flat::geometry::Vector2 position2d = xAxis * m_x + yAxis * m_y + zAxis * m_z;
	m_sprite.setPosition(position2d);
	if (m_doodadSprite)
	{
		m_doodadSprite->setPosition(position2d);
	}
}

void Tile::setTexture(std::shared_ptr<const flat::video::Texture> tileTexture)
{
	FLAT_ASSERT(m_exists);
	m_sprite.setTexture(tileTexture);
	const flat::geometry::Vector2& textureSize = tileTexture->getSize();
	flat::geometry::Vector2 origin(textureSize.x / 2, textureSize.x / 4); // should depend on tile width/height instead
	m_sprite.setOrigin(origin);
}

void Tile::setDoodadTexture(std::shared_ptr<const flat::video::Texture> doodadTexture)
{
	FLAT_ASSERT(m_exists);
	if (!m_doodadSprite)
	{
		m_doodadSprite = new flat::util::Sprite();
		m_doodadSprite->setPosition(m_sprite.getPosition());
	}
	m_doodadSprite->setTexture(doodadTexture);
	const flat::geometry::Vector2& textureSize = doodadTexture->getSize();
	flat::geometry::Vector2 origin(textureSize.x / 2, textureSize.y - textureSize.x / 4);
	m_doodadSprite->setOrigin(origin);
}

void Tile::removeDoodad()
{
	FLAT_DELETE(m_doodadSprite);
}

} // map
} // game



