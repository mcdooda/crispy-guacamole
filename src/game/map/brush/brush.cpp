#include "brush.h"
#include "../map.h"
#include "../tile.h"

namespace game
{
namespace map
{
namespace brush
{

Brush::Brush() :
	m_radius(1.f),
	m_edgeWidth(0.f)
{
	
}

void Brush::getTiles(Map& map, const flat::geometry::Vector2& center, std::vector<map::Tile*>& tiles, float minEffect) const
{
	const int minX = floor(center.x - m_radius);
	const int maxX = ceil(center.x + m_radius);
	const int minY = floor(center.y - m_radius);
	const int maxY = ceil(center.y + m_radius);
	
	tiles.clear();
	tiles.reserve((maxX - minX) * (maxY - minY));
	
	flat::geometry::Vector2 positionFromCenter;
	
	for (int x = minX; x <= maxX; ++x)
	{
		for (int y = minY; y <= maxY; ++y)
		{
			if (map::Tile* tile = map.getTileIfExists(x, y))
			{
				positionFromCenter.x = static_cast<float>(x) - center.x;
				positionFromCenter.y = static_cast<float>(y) - center.y;
				float effect = getEffect(positionFromCenter);
				if (effect > minEffect)
				{
					tiles.push_back(tile);
				}
			}
		}
	}
}

float Brush::getTileEffect(const flat::geometry::Vector2& center, const map::Tile* tile) const
{
	flat::geometry::Vector2 positionFromCenter(
		static_cast<float>(tile->getX()) - center.x,
		static_cast<float>(tile->getY()) - center.y
	);
	return getEffect(positionFromCenter);
}

void Brush::getEntities(Map& map, const flat::geometry::Vector2& center, std::vector<entity::Entity*>& entities, float minEffect) const
{
	const int minX = floor(center.x - m_radius);
	const int maxX = ceil(center.x + m_radius);
	const int minY = floor(center.y - m_radius);
	const int maxY = ceil(center.y + m_radius);
	
	entities.clear();
	
	flat::geometry::Vector2 positionFromCenter;
	
	for (int x = minX; x <= maxX; ++x)
	{
		for (int y = minY; y <= maxY; ++y)
		{
			if (map::Tile* tile = map.getTileIfExists(x, y))
			{
				float effect = getTileEffect(center, tile);
				if (effect > minEffect)
				{
					const std::vector<entity::Entity*> tileEntities = tile->getEntities();
					entities.insert(entities.end(), tileEntities.begin(), tileEntities.end());
				}
			}
		}
	}
}

} // brush
} // map
} // game


