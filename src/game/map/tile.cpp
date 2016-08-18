#include "tile.h"
#include "map.h"

namespace game
{
namespace map
{

Tile::Tile() :
	m_propSprite(nullptr),
	m_exists(true),
	m_walkable(true)
{
	m_sprite.setColor(flat::video::Color::WHITE);
}

Tile::~Tile()
{
	FLAT_DELETE(m_propSprite);
}

void Tile::draw(const flat::util::RenderSettings& renderSettings, const flat::geometry::Matrix4& viewMatrix) const
{
	FLAT_ASSERT(m_exists);
	m_sprite.draw(renderSettings, viewMatrix);
	if (m_propSprite)
	{
		// TODO prop depth != tile depth
		m_propSprite->draw(renderSettings, viewMatrix);
	}
}

void Tile::setCoordinates(const Map& map, int x, int y, float z)
{
	FLAT_ASSERT(m_exists);
	m_x = x;
	m_y = y;
	m_z = z;
	computeDepth(x, y, -0.5f);
	
	const flat::geometry::Vector2& xAxis = map.getXAxis();
	const flat::geometry::Vector2& yAxis = map.getYAxis();
	const flat::geometry::Vector2& zAxis = map.getZAxis();
	
	flat::geometry::Vector2 position2d = xAxis * m_x + yAxis * m_y + zAxis * m_z;
	m_sprite.setPosition(position2d);
	if (m_propSprite)
	{
		m_propSprite->setPosition(position2d);
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

void Tile::setPropTexture(std::shared_ptr<const flat::video::Texture> propTexture)
{
	FLAT_ASSERT(m_exists);
	if (!m_propSprite)
	{
		m_propSprite = new flat::util::Sprite();
		m_propSprite->setPosition(m_sprite.getPosition());
	}
	m_propSprite->setTexture(propTexture);
	const flat::geometry::Vector2& textureSize = propTexture->getSize();
	flat::geometry::Vector2 origin(textureSize.x / 2, textureSize.y - textureSize.x / 4);
	m_propSprite->setOrigin(origin);
	setWalkable(false);
}

void Tile::removeProp()
{
	FLAT_DELETE(m_propSprite);
	setWalkable(true);
}

void Tile::addEntity(entity::Entity* entity)
{
	FLAT_ASSERT(std::find(m_entities.begin(), m_entities.end(), entity) == m_entities.end());
	m_entities.push_back(entity);
}

void Tile::removeEntity(entity::Entity* entity)
{
	std::vector<entity::Entity*>::iterator it = std::find(m_entities.begin(), m_entities.end(), entity);
	FLAT_ASSERT(it != m_entities.end());
	m_entities.erase(it);
}

void Tile::setColor(const flat::video::Color& color)
{
	m_sprite.setColor(color);
	if (m_propSprite)
	{
		m_propSprite->setColor(color);
	}
}

void Tile::getWalkableNeighborTiles(const Map& map, float jumpHeight, std::vector<const Tile*>& neighborTiles) const
{
	neighborTiles.clear();
	neighborTiles.reserve(4);
	
	const float maxZ = m_z + jumpHeight;
	
	if (const Tile* tile = map.getTileIfWalkable(m_x - 1, m_y))
	{
		if (tile->getZ() <= maxZ)
			neighborTiles.push_back(tile);
	}
		
	if (const Tile* tile = map.getTileIfWalkable(m_x + 1, m_y))
	{
		if (tile->getZ() <= maxZ)
			neighborTiles.push_back(tile);
	}
		
	if (const Tile* tile = map.getTileIfWalkable(m_x, m_y - 1))
	{
		if (tile->getZ() <= maxZ)
			neighborTiles.push_back(tile);
	}
		
	if (const Tile* tile = map.getTileIfWalkable(m_x, m_y + 1))
	{
		if (tile->getZ() <= maxZ)
			neighborTiles.push_back(tile);
	}
}

} // map
} // game



