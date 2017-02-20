#include "zone.h"
#include "map.h"

namespace game
{
namespace map
{

Zone::Zone(Map& map) :
	m_color(flat::video::Color::BLUE),
	m_map(map)
{

}

void Zone::addRectangle(const Rectangle& rectangle)
{
	FLAT_ASSERT(isValidRectangle(rectangle));

	eachRectangleTileIfExists(rectangle, [this](Tile* tile)
	{
		m_tiles.insert(tile);
	});
}

bool Zone::isInside(const flat::Vector2& point) const
{
	Tile* tile = m_map.getTileIfExists(point.x, point.y);
	return tile != nullptr && m_tiles.count(tile) > 0;
}

void Zone::eachTileIfExists(std::function<void(Tile*)> func)
{
	for (Tile* tile : m_tiles)
	{
		func(tile);
	}
}

void Zone::eachRectangleTileIfExists(const Rectangle& rectangle, std::function<void(Tile*)> func)
{
	FLAT_ASSERT(isValidRectangle(rectangle));

	for (int x = rectangle.minX; x <= rectangle.maxX; ++x)
	{
		for (int y = rectangle.minY; y <= rectangle.maxY; ++y)
		{
			Tile* tile = m_map.getTileIfExists(x, y);
			if (tile != nullptr)
			{
				func(tile);
			}
		}
	}
}

bool Zone::isValidRectangle(const Rectangle& rectangle)
{
	return rectangle.minX <= rectangle.maxX && rectangle.minY <= rectangle.maxY;
}

} // map
} // game


