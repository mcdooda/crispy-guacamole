#include "editormap.h"
#include "tile.h"

namespace game
{
namespace map
{

EditorMap::EditorMap()
{

}

EditorMap::~EditorMap()
{
}

Tile* EditorMap::getTile(int x, int y)
{
	m_minX = std::min(m_minX, x);
	m_maxX = std::max(m_maxX, x);
	m_minY = std::min(m_minY, y);
	m_maxY = std::max(m_maxY, y);
	TilePosition tilePosition{ x, y };
	std::map<TilePosition, Tile>::iterator it = m_tiles.find(tilePosition);
	if (it == m_tiles.end())
	{
		Tile& tile = m_tiles[tilePosition];
		tile.setCoordinates(*this, x, y, 0.f);
		tile.setExists(false);
		return &tile;
	}
	return &it->second;
}

void EditorMap::eachTile(std::function<void(Tile*)> func)
{
	for (std::pair<const TilePosition, Tile>& pair : m_tiles)
	{
		func(&pair.second);
	}
}

void EditorMap::createTiles()
{
	FLAT_ASSERT(m_minX < m_maxX && m_minY < m_maxY);
	for (int y = m_minY; y <= m_maxY; ++y)
	{
		for (int x = m_minX; x <= m_maxX; ++x)
		{
			TilePosition tilePosition{ x, y };
			Tile& tile = m_tiles[tilePosition];
			tile.setCoordinates(*this, x, y, 0.f);
			tile.setExists(false);
		}
	}
}

} // map
} // game


