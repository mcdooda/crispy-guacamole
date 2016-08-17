#include "map.h"
#include "displaymanager.h"
#include "tile.h"
#include "io/reader.h"
#include "../mod/mod.h"
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
	destroyEntities();
	destroyTiles();
}

bool Map::load(Game* game, const mod::Mod& mod)
{
	io::Reader reader(game, mod, *this);
	if (reader.canRead())
	{
		// TODO read from map.gpmap
		setAxes(
			mod.getXAxis(),
			mod.getYAxis(),
			mod.getZAxis()
		);
		reader.read();
		return true;
	}
	return false;
}

void Map::createEmptyMap(const mod::Mod& mod)
{
	setSize(mod.getMapWidth(), mod.getMapHeight());
	setAxes(
		mod.getXAxis(),
		mod.getYAxis(),
		mod.getZAxis()
	);
	createTiles();
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
	return const_cast<Tile*>((const_cast<const Map*>(this)->getTile(x, y)));
}

const Tile* Map::getTileIfExists(int x, int y) const
{
	const Tile* tile = getTile(x, y);
	
	if (!tile || !tile->exists())
		return nullptr;
		
	return tile;
}

Tile* Map::getTileIfExists(int x, int y)
{
	return const_cast<Tile*>((const_cast<const Map*>(this)->getTileIfExists(x, y)));
}

const Tile* Map::getTileIfWalkable(int x, int y) const
{
	const Tile* tile = getTileIfExists(x, y);
	
	if (!tile || !tile->isWalkable())
		return nullptr;
		
	return tile;
}

Tile* Map::getTileIfWalkable(int x, int y)
{
	return const_cast<Tile*>((const_cast<const Map*>(this)->getTileIfWalkable(x, y)));
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

void Map::updateEntities(float currentTime, float elapsedTime)
{
	for (entity::Entity* entity : m_entities)
	{
		entity->update(currentTime, elapsedTime);
	}
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

void Map::setAxes(const flat::geometry::Vector2& xAxis,
                  const flat::geometry::Vector2& yAxis,
                  const flat::geometry::Vector2& zAxis)
{
	m_xAxis = xAxis;
	m_yAxis = yAxis;
	m_zAxis = zAxis;
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
			tile->setExists(false);
		}
	}
}

void Map::destroyTiles()
{
	FLAT_DELETE_ARRAY(m_tiles);
	m_width = 0;
	m_height = 0;
}

void Map::destroyEntities()
{
	for (entity::Entity*& entity : m_entities)
	{
		FLAT_DELETE(entity);
	}
	m_entities.clear();
}

} // map
} // game



