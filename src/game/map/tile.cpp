#include "tile.h"
#include "map.h"
#include "prop.h"

namespace game
{
namespace map
{

Tile::Tile() :
	m_prop(nullptr),
	m_exists(true),
	m_walkable(true)
{
	m_sprite.setColor(flat::video::Color::WHITE);
}

Tile::~Tile()
{
	FLAT_DELETE(m_prop);
}

const flat::util::Sprite& Tile::getSprite() const
{
	FLAT_ASSERT(m_exists);
	return m_sprite;
}

void Tile::setCoordinates(const Map& map, int x, int y, float z)
{
	FLAT_ASSERT(m_exists);
	m_x = x;
	m_y = y;
	m_z = z;
	computeDepth(static_cast<float>(x), static_cast<float>(y), -0.5f);
	
	const flat::Vector2& xAxis = map.getXAxis();
	const flat::Vector2& yAxis = map.getYAxis();
	const flat::Vector2& zAxis = map.getZAxis();
	
	flat::Vector2 position2d = xAxis * static_cast<float>(m_x) + yAxis * static_cast<float>(m_y) + zAxis * static_cast<float>(m_z);
	m_sprite.setPosition(position2d);
	if (m_prop)
	{
		m_prop->setSpritePosition(position2d);
		m_prop->computeDepth(static_cast<float>(x), static_cast<float>(y), -0.49f);
	}
}

void Tile::setTexture(const std::shared_ptr<const flat::video::Texture>& tileTexture)
{
	FLAT_ASSERT(m_exists);
	m_sprite.setTexture(tileTexture);
	setTextureHash(tileTexture.get()->getHash());
	const flat::Vector2& textureSize = tileTexture->getSize();
	flat::Vector2 origin(textureSize.x / 2, textureSize.x / 4); // should depend on tile width/height instead
	m_sprite.setOrigin(origin);
}

void Tile::setPropTexture(const std::shared_ptr<const flat::video::Texture>& propTexture)
{
	FLAT_ASSERT(m_exists);
	if (!m_prop)
	{
		m_prop = new Prop();
		m_prop->setSpritePosition(m_sprite.getPosition());
		m_prop->computeDepth(static_cast<float>(m_x), static_cast<float>(m_y), -0.49f);
	}
	m_prop->setSpriteTexture(propTexture);
	m_prop->setTextureHash(propTexture.get()->getHash());
	const flat::Vector2& textureSize = propTexture->getSize();
	flat::Vector2 origin(textureSize.x / 2.f, textureSize.y - textureSize.x / 4.f);
	m_prop->setSpriteOrigin(origin);
	setWalkable(false);
}

void Tile::removeProp()
{
	FLAT_DELETE(m_prop);
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
	if (m_prop)
	{
		m_prop->setSpriteColor(color);
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



