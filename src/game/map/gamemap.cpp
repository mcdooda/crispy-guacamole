#include "gamemap.h"
#include "tile.h"

namespace game
{
namespace map
{

GameMap::GameMap() :
	m_tiles(nullptr)
{

}

GameMap::~GameMap()
{
	destroyTiles();
}

Tile* GameMap::getTile(int x, int y)
{
	int tileIndex = getTileIndex(x, y);

	if (tileIndex < 0)
		return nullptr;

	FLAT_ASSERT(tileIndex < getNumTiles());
	return &m_tiles[tileIndex];
}

void GameMap::eachTile(std::function<void(Tile*)> func)
{
	if (m_tiles != nullptr)
	{
		Tile* end = m_tiles + getNumTiles();
		for (Tile* tile = m_tiles; tile < end; ++tile)
		{
			func(tile);
		}
	}
}

void GameMap::createTiles()
{
	FLAT_ASSERT(m_tiles == nullptr);
	FLAT_ASSERT(m_minX < m_maxX && m_minY < m_maxY);
	m_tiles = new Tile[getNumTiles()];
	for (int y = m_minY; y <= m_maxY; ++y)
	{
		for (int x = m_minX; x <= m_maxX; ++x)
		{
			Tile* tile = getTile(x, y);
			tile->setCoordinates(*this, x, y, 0.f);
		}
	}
}

void GameMap::destroyTiles()
{
	FLAT_DELETE_ARRAY(m_tiles);
	m_minX = 0;
	m_maxX = 0;
	m_minY = 0;
	m_maxY = 0;
}

int GameMap::getTileIndex(int x, int y) const
{
	if (x < m_minX || x > m_maxX || y < m_minY || y > m_maxY)
		return -1;

	return getWidth() * (y - m_minY) + (x - m_minX);
}

int GameMap::getWidth() const
{
	return m_maxX - m_minX + 1;
}

int GameMap::getHeight() const
{
	return m_maxY - m_minY + 1;
}

int GameMap::getNumTiles() const
{
	return getWidth() * getHeight();
}

} // map
} // game


