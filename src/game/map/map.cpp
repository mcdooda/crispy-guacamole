#include "map.h"
#include "displaymanager.h"
#include "tile.h"
#include "prop.h"
#include "io/reader.h"
#include "io/writer.h"
#include "../mod/mod.h"
#include "../entity/entity.h"

namespace game
{
namespace map
{

Map::Map() :
	m_width(0),
	m_height(0),
	m_tiles(nullptr)
{
	
}

Map::~Map()
{
	FLAT_ASSERT(m_entities.empty());
	destroyTiles();
}

void Map::operator=(Map&& other)
{
	if (m_tiles != nullptr)
	{
		FLAT_DELETE_ARRAY(m_tiles);
	}
	m_tiles = other.m_tiles;
	other.m_tiles = nullptr;

	m_width = other.m_width;
	m_height = other.m_height;
	other.m_width = 0;
	other.m_height = 0;

	m_xAxis = other.m_xAxis;
	m_yAxis = other.m_yAxis;
	m_zAxis = other.m_zAxis;

	m_entities = std::move(other.m_entities);
}

bool Map::load(lua_State* L, Game& game, const mod::Mod& mod, const std::string& mapName)
{
	io::Reader reader(game, mod, mapName, *this);
	if (reader.canRead())
	{
		reader.read(L);
		return true;
	}
	FLAT_ASSERT(false);
	return false;
}

bool Map::save(const mod::Mod& mod, const std::string& mapName) const
{
	io::Writer writer(mod, mapName, *this);
	if (writer.canWrite())
	{
		writer.write();
		return true;
	}
	return false;
}

void Map::drawTiles(DisplayManager& displayManager, const flat::video::View& view) const
{
	eachTileTopToDown([&displayManager](const Tile* tile)
	{
		if (tile->exists())
		{
			displayManager.add(tile);
			if (const Prop* prop = tile->getProp())
			{
				displayManager.add(prop);
			}
			else
			{
				for (entity::Entity* entity : tile->getEntities())
				{
					displayManager.add(entity);
				}
			}
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
	return const_cast<Tile*>(const_cast<const Map*>(this)->getTileIfExists(x, y));
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
	return const_cast<Tile*>(const_cast<const Map*>(this)->getTileIfWalkable(x, y));
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

void Map::eachTileTopToDown(std::function<void(const Tile*)> func) const
{
	int size = m_width + m_height;
	for (int i = 0; i < size; ++i)
	{
		for (int x = i, y = 0; x >= 0 && y < m_height; --x, ++y)
		{
			if (const Tile* tile = getTile(x, y))
			{
				func(tile);
			}
		}
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

void Map::eachEntityInRange(const flat::Vector2& center2d, float range, std::function<void(entity::Entity*)> func) const
{
	const int tileMinX = static_cast<int>(std::round(center2d.x - range));
	const int tileMinY = static_cast<int>(std::round(center2d.y - range));
	const int tileMaxX = static_cast<int>(std::round(center2d.x + range));
	const int tileMaxY = static_cast<int>(std::round(center2d.y + range));
	const float range2 = range * range;
	for (int x = tileMinX; x <= tileMaxX; ++x)
	{
		for (int y = tileMinY; y <= tileMaxY; ++y)
		{
			if (const map::Tile* tile = getTileIfWalkable(x, y))
			{
				for (entity::Entity* entity : tile->getEntities())
				{
					flat::Vector2 entityPosition2d(entity->getPosition());
					if (flat::length2(center2d - entityPosition2d) <= range2)
					{
						func(entity);
					}
				}
			}
		}
	}
}

void Map::updateEntities(float currentTime, float elapsedTime)
{
	std::vector<entity::Entity*> entities = m_entities;
	for (entity::Entity* entity : entities)
	{
		entity->update(currentTime, elapsedTime);
	}
}

#ifdef FLAT_DEBUG
void Map::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	for (entity::Entity* entity : m_entities)
	{
		entity->debugDraw(debugDisplay);
	}
}
#endif

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

void Map::setAxes(const flat::Vector2& xAxis,
                  const flat::Vector2& yAxis,
                  const flat::Vector2& zAxis)
{
	m_transform = flat::Matrix3(flat::Vector3(xAxis, 1.f), flat::Vector3(yAxis, 1.f), flat::Vector3(zAxis, 1.f));
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

} // map
} // game



