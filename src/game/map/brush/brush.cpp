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
			map::TileIndex tileIndex = map.getTileIndex(x, y);
			if (map::isValidTile(tileIndex))
			{
				positionFromCenter.x = static_cast<float>(x) - center.x;
				positionFromCenter.y = static_cast<float>(y) - center.y;
				float effect = getEffect(positionFromCenter);
				if (effect > minEffect)
				{
					tiles.emplace_back(tileIndex, effect);
				}
			}
		}
	}
}

void Brush::getTileSlots(Map& map, const flat::Vector2& center, TileSlotsContainer& tileSlots, float minEffect /*= 0.f*/) const
{
	const int minX = static_cast<int>(std::floor(center.x - m_radius));
	const int maxX = static_cast<int>(std::ceil(center.x + m_radius));
	const int minY = static_cast<int>(std::floor(center.y - m_radius));
	const int maxY = static_cast<int>(std::ceil(center.y + m_radius));

	tileSlots.clear();
	tileSlots.reserve((maxX - minX) * (maxY - minY));

	flat::Vector2 positionFromCenter;

	for (int x = minX; x <= maxX; ++x)
	{
		for (int y = minY; y <= maxY; ++y)
		{
			positionFromCenter.x = static_cast<float>(x) - center.x;
			positionFromCenter.y = static_cast<float>(y) - center.y;
			float effect = getEffect(positionFromCenter);
			if (effect > minEffect)
			{
				tileSlots.emplace_back(flat::Vector2i(x, y), effect);
			}
		}
	}
}

float Brush::getTileEffect(const Map& map, const flat::Vector2& center, TileIndex tileIndex) const
{
	const flat::Vector2i& xy = map.getTileXY(tileIndex);
	flat::Vector2 positionFromCenter(
		static_cast<float>(xy.x) - center.x,
		static_cast<float>(xy.y) - center.y
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
			const map::TileIndex tileIndex = map.getTileIndex(x, y);
			if (map::isValidTile(tileIndex))
			{
				float effect = getTileEffect(map, center, tileIndex);
				if (effect > minEffect)
				{
					map.eachTileEntity(
						tileIndex,
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



