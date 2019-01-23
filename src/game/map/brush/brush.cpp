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

void Brush::getTiles(Map& map, const flat::Vector2& center, TilesContainer& tiles, float minEffect) const
{
	const int minX = static_cast<int>(std::floor(center.x - m_radius));
	const int maxX = static_cast<int>(std::ceil(center.x + m_radius));
	const int minY = static_cast<int>(std::floor(center.y - m_radius));
	const int maxY = static_cast<int>(std::ceil(center.y + m_radius));
	
	tiles.clear();
	tiles.reserve((maxX - minX) * (maxY - minY));
	
	flat::Vector2 positionFromCenter;
	
	for (int x = minX; x <= maxX; ++x)
	{
		for (int y = minY; y <= maxY; ++y)
		{
			if (map::Tile* tile = map.getTile(x, y))
			{
				positionFromCenter.x = static_cast<float>(x) - center.x;
				positionFromCenter.y = static_cast<float>(y) - center.y;
				float effect = getEffect(positionFromCenter);
				if (effect > minEffect)
				{
					tiles.emplace_back(tile, effect);
				}
			}
		}
	}
}

float Brush::getTileEffect(const flat::Vector2& center, const map::Tile* tile) const
{
	flat::Vector2 positionFromCenter(
		static_cast<float>(tile->getX()) - center.x,
		static_cast<float>(tile->getY()) - center.y
	);
	return getEffect(positionFromCenter);
}

void Brush::getEntities(Map& map, const flat::Vector2& center, std::vector<entity::Entity*>& entities, float minEffect) const
{
	const int minX = static_cast<int>(std::floor(center.x - m_radius));
	const int maxX = static_cast<int>(std::ceil(center.x + m_radius));
	const int minY = static_cast<int>(std::floor(center.y - m_radius));
	const int maxY = static_cast<int>(std::ceil(center.y + m_radius));
	
	entities.clear();
	
	for (int x = minX; x <= maxX; ++x)
	{
		for (int y = minY; y <= maxY; ++y)
		{
			if (map::Tile* tile = map.getTileIfExists(x, y))
			{
				float effect = getTileEffect(center, tile);
				if (effect > minEffect)
				{
					map.eachTileEntity(
						tile,
						[&entities](entity::Entity* entity)
						{
							entities.push_back(entity);
						}
					);
				}
			}
		}
	}
}

} // brush
} // map
} // game



