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
		tile.setExists(*this, false);
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
	// nothing to do as tiles are created on the fly
}

} // map
} // game


