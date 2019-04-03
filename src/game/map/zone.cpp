#include "zone.h"
#include "map.h"
#include "../game.h"

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

	eachRectangleTile(rectangle, [this](TileIndex tileIndex)
	{
		m_tiles.insert(tileIndex);
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
		eachRectangleTile(rectangle, [this](TileIndex tileIndex)
		{
			m_tiles.insert(tileIndex);
		});
	}
}

bool Zone::isInside(const flat::Vector2& point) const
{
	TileIndex tileIndex = m_map.getTileIndex(point.x, point.y);
	return tileIndex != TileIndex::INVALID_TILE && m_tiles.count(tileIndex) > 0;
}

void Zone::eachTile(std::function<void(TileIndex tileIndex)> func)
{
	for (TileIndex tileIndex : m_tiles)
	{
		func(tileIndex);
	}
}

TileIndex Zone::getRandomTile(Game& game) const
{
	if (m_tiles.empty())
		return TileIndex::INVALID_TILE;

	int randomIndex = game.random->nextInt(0, static_cast<int>(m_tiles.size()) - 1);
	std::set<TileIndex>::iterator it = m_tiles.begin();
	std::advance(it, randomIndex);
	TileIndex tileIndex = *it;
	FLAT_ASSERT(tileIndex != TileIndex::INVALID_TILE);
	return tileIndex;
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

flat::Vector2 Zone::getCenter() const
{
	FLAT_ASSERT_MSG(m_rectangles.size() == 1, "Cannot get center of complex zones (it must have exactly 1 rectangle)");
	flat::Vector2 center;
	const Rectangle& rectangle = m_rectangles[0];
	center.x = (rectangle.minX + rectangle.maxX) / 2.f;
	center.y = (rectangle.minY + rectangle.maxY) / 2.f;
	return center;
}

void Zone::eachRectangleTile(const Rectangle& rectangle, std::function<void(TileIndex)> func)
{
	FLAT_ASSERT(isValidRectangle(rectangle));

	for (int x = rectangle.minX; x <= rectangle.maxX; ++x)
	{
		for (int y = rectangle.minY; y <= rectangle.maxY; ++y)
		{
			TileIndex tileIndex = m_map.getTileIndex(x, y);
			if (tileIndex != TileIndex::INVALID_TILE)
			{
				func(tileIndex);
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


