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
	m_minX(0),
	m_maxX(0),
	m_minY(0),
	m_maxY(0)
{
	
}

Map::~Map()
{
	FLAT_ASSERT(m_entities.empty());
}

bool Map::load(Game& game, const mod::Mod& mod, const std::string& mapName)
{
	io::Reader reader(game, mod, mapName, *this);
	if (reader.canRead())
	{
		reader.read();
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

void Map::setBounds(int minX, int maxX, int minY, int maxY)
{
	m_minX = minX;
	m_maxX = maxX;
	m_minY = minY;
	m_maxY = maxY;
}

void Map::getBounds(int& minX, int& maxX, int& minY, int& maxY) const
{
	minX = m_minX;
	maxX = m_maxX;
	minY = m_minY;
	maxY = m_maxY;
}

void Map::getActualBounds(int& minX, int& maxX, int& minY, int& maxY) const
{
	// note that min and max are swapped
	getBounds(maxX, minX, maxY, minY);

	eachTile([&](const Tile* tile)
	{
		if (tile->exists())
		{
			const int x = tile->getX();
			const int y = tile->getY();
			minX = std::min(minX, x);
			maxX = std::max(maxX, x);
			minY = std::min(minY, y);
			maxY = std::max(maxY, y);
		}
	});
}

void Map::drawTiles(DisplayManager& displayManager, const flat::video::View& view) const
{
	eachTileIfExists([&displayManager](const Tile* tile)
	{
		displayManager.add(tile);
		if (const Prop* prop = tile->getProp())
		{
			displayManager.add(prop);
		}
		for (entity::Entity* entity : tile->getEntities())
		{
			displayManager.add(entity);
		}
	});
}

const Tile* Map::getTile(int x, int y) const
{
	return const_cast<Map*>(this)->getTile(x, y);
}

const Tile* Map::getTile(float x, float y) const
{
	return const_cast<Map*>(this)->getTile(x, y);
}

Tile* Map::getTile(float x, float y)
{
	return getTile(static_cast<int>(std::round(x)), static_cast<int>(std::round(y)));
}

const Tile* Map::getTileIfExists(int x, int y) const
{
	return const_cast<Map*>(this)->getTileIfExists(x, y);
}

Tile* Map::getTileIfExists(int x, int y)
{
	Tile* tile = getTile(x, y);

	if (!tile || !tile->exists())
		return nullptr;

	return tile;
}

const Tile* Map::getTileIfExists(float x, float y) const
{
	return const_cast<Map*>(this)->getTileIfExists(x, y);
}

Tile* Map::getTileIfExists(float x, float y)
{
	return getTileIfExists(static_cast<int>(std::round(x)), static_cast<int>(std::round(y)));
}

const Tile* Map::getTileIfWalkable(int x, int y) const
{
	return const_cast<Map*>(this)->getTileIfWalkable(x, y);
}

Tile* Map::getTileIfWalkable(int x, int y)
{
	Tile* tile = getTileIfExists(x, y);

	if (!tile || !tile->isWalkable())
		return nullptr;

	return tile;
}

const Tile* Map::getTileIfWalkable(float x, float y) const
{
	return const_cast<Map*>(this)->getTileIfWalkable(x, y);
}

Tile* Map::getTileIfWalkable(float x, float y)
{
	return getTileIfWalkable(static_cast<int>(std::round(x)), static_cast<int>(std::round(y)));;
}

void Map::eachTile(std::function<void(const Tile*)> func) const
{
	const_cast<Map*>(this)->eachTile([func](Tile* tile)
	{
		func(tile);
	});
}

void Map::eachTileIfExists(std::function<void(const Tile*)> func) const
{
	const_cast<Map*>(this)->eachTileIfExists([func](Tile* tile)
	{
		func(tile);
	});
}

void Map::eachTileIfExists(std::function<void(Tile*)> func)
{
	eachTile([func](Tile* tile)
	{
		if (tile->exists())
		{
			func(tile);
		}
	});
}

void Map::addEntity(entity::Entity* entity)
{
	FLAT_ASSERT(entity != nullptr);
	FLAT_ASSERT(std::find(m_entities.begin(), m_entities.end(), entity) == m_entities.end());
	m_entities.push_back(entity);
	entity->onAddedToMap(this);
}

void Map::removeEntity(entity::Entity* entity)
{
	FLAT_ASSERT(entity != nullptr);
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

void Map::setAxes(const flat::Vector2& xAxis,
                  const flat::Vector2& yAxis,
                  const flat::Vector2& zAxis)
{
	m_transform = flat::Matrix3(flat::Vector3(xAxis, 1.f), flat::Vector3(yAxis, 1.f), flat::Vector3(zAxis, 1.f));
	m_invTransform = flat::inverse(m_transform);
	m_xAxis = xAxis;
	m_yAxis = yAxis;
	m_zAxis = zAxis;
}

} // map
} // game



