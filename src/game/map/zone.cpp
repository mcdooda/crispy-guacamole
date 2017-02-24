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

const Zone::Rectangle& Zone::addRectangle(const Rectangle& rectangle)
{
	FLAT_ASSERT(isValidRectangle(rectangle));

	m_rectangles.push_back(rectangle);

	eachRectangleTileIfExists(rectangle, [this](const Tile* tile)
	{
		m_tiles.insert(tile);
	});

	return m_rectangles.back();
}

void Zone::removeRectangle(const Rectangle& rectangle)
{
	ptrdiff_t index = &rectangle - &m_rectangles[0];
	FLAT_ASSERT(index >= 0 && index < static_cast<ptrdiff_t>(m_rectangles.size()));
	std::vector<Rectangle>::iterator it = m_rectangles.begin() + index;
	m_rectangles.erase(it);

	m_tiles.clear();
	for (const Rectangle& rectangle : m_rectangles)
	{
		eachRectangleTileIfExists(rectangle, [this](const Tile* tile)
		{
			m_tiles.insert(tile);
		});
	}
}

bool Zone::isInside(const flat::Vector2& point) const
{
	const Tile* tile = m_map.getTileIfExists(point.x, point.y);
	return tile != nullptr && m_tiles.count(tile) > 0;
}

void Zone::eachTileIfExists(std::function<void(const Tile*)> func)
{
	for (const Tile* tile : m_tiles)
	{
		func(tile);
	}
}

const Zone::Rectangle* Zone::selectRectangle(const flat::Vector2& position) const
{
	int x = static_cast<int>(std::round(position.x));
	int y = static_cast<int>(std::round(position.y));

	const Rectangle* smallestRectangle = nullptr;
	for (const Rectangle& rectangle : m_rectangles)
	{
		if (rectangle.minX <= x && x <= rectangle.maxX
		 && rectangle.minY <= y && y <= rectangle.maxY)
		{
			if (smallestRectangle == nullptr
				|| std::min(rectangle.maxX - rectangle.minX, rectangle.maxY - rectangle.minY)
				 < std::min(smallestRectangle->maxX - smallestRectangle->minX, smallestRectangle->maxY - smallestRectangle->minY))
			{
				smallestRectangle = &rectangle;
			}
		}
	}

	return smallestRectangle;
}

void Zone::eachRectangleTileIfExists(const Rectangle& rectangle, std::function<void(const Tile*)> func)
{
	FLAT_ASSERT(isValidRectangle(rectangle));

	for (int x = rectangle.minX; x <= rectangle.maxX; ++x)
	{
		for (int y = rectangle.minY; y <= rectangle.maxY; ++y)
		{
			const Tile* tile = m_map.getTileIfExists(x, y);
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


