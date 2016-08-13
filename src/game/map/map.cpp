#include "map.h"
#include "displaymanager.h"
#include "tile.h"
#include "io/reader.h"
#include "../entity/entity.h"

namespace game
{
namespace map
{

Map::Map() :
	m_width(0),
	m_height(0),
	m_tileWidth(0),
	m_tileHeight(0),
	m_tiles(nullptr)
{
	
}

Map::~Map()
{
	destroyTiles();
}

void Map::load(Game* game, const mod::Mod& mod)
{
	io::Reader reader(game, mod, *this);
	reader.read();
}

void Map::drawTiles(DisplayManager& displayManager, const flat::video::View& view) const
{
	eachTile([&displayManager](const Tile* tile)
	{
		if (tile->exists())
		{
			displayManager.add(tile);
			for (entity::Entity* entity : tile->getEntities())
				displayManager.add(entity);
		}
	});
}

const Tile* Map::getTile(int x, int y) const
{
	int tileIndex = getTileIndex(x, y);
	
	if (tileIndex < 0)
		return nullptr;
	
	FLAT_ASSERT(tileIndex < getNumTiles());
	return &m_tiles[tileIndex];
}

Tile* Map::getTile(int x, int y)
{
	int tileIndex = getTileIndex(x, y);
	
	if (tileIndex < 0)
		return nullptr;
	
	FLAT_ASSERT(tileIndex < getNumTiles());
	return &m_tiles[tileIndex];
}

const Tile* Map::getTileIfExists(int x, int y) const
{
	const Tile* tile = getTile(x, y);
	
	if (!tile->exists())
		return nullptr;
		
	return tile;
}

Tile* Map::getTileIfExists(int x, int y)
{
	Tile* tile = getTile(x, y);
	
	if (tile && !tile->exists())
		return nullptr;
		
	return tile;
}

void Map::eachTile(std::function<void(const Tile*)> func) const
{
	const Tile* end = m_tiles + getNumTiles();
	for (const Tile* tile = m_tiles; tile < end; ++tile)
	{
		func(tile);
	}
}

void Map::eachTile(std::function<void(Tile*)> func)
{
	Tile* end = m_tiles + getNumTiles();
	for (Tile* tile = m_tiles; tile < end; ++tile)
	{
		func(tile);
	}
}

flat::geometry::Vector2 Map::getXAxis() const
{
	return flat::geometry::Vector2(-m_tileWidth / 2.f, m_tileHeight / 2.f);
}

flat::geometry::Vector2 Map::getYAxis() const
{
	return flat::geometry::Vector2(m_tileWidth / 2.f, m_tileHeight / 2.f);
}

flat::geometry::Vector2 Map::getZAxis() const
{
	return flat::geometry::Vector2(0.f, -m_tileHeight);
}

void Map::addEntity(entity::Entity* entity)
{
	FLAT_ASSERT(std::find(m_entities.begin(), m_entities.end(), entity) == m_entities.end());
	m_entities.push_back(entity);
	entity->onAddedToMap(this);
}

void Map::removeEntity(entity::Entity* entity)
{
	std::vector<entity::Entity*>::iterator it = std::find(m_entities.begin(), m_entities.end(), entity);
	FLAT_ASSERT(it != m_entities.end());
	m_entities.erase(it);
	entity->onRemovedFromMap();
}

int Map::getTileIndex(int x, int y) const
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
		return -1;
	
	return m_width * y + x;
}

int Map::getNumTiles() const
{
	return m_width * m_height;
}

void Map::setSize(int width, int height)
{
	m_width = width;
	m_height = height;
}

void Map::setTileSize(int tileWidth, int tileHeight)
{
	m_tileWidth = tileWidth;
	m_tileHeight = tileHeight;
}

void Map::createTiles()
{
	FLAT_ASSERT(m_tiles == nullptr);
	FLAT_ASSERT(m_width > 0 && m_height > 0);
	m_tiles = new Tile[m_width * m_height];
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			Tile* tile = getTile(x, y);
			tile->setCoordinates(*this, x, y, 0.f);
		}
	}
}

void Map::destroyTiles()
{
	FLAT_DELETE_ARRAY(m_tiles);
	m_width = 0;
	m_height = 0;
}

} // map
} // game



