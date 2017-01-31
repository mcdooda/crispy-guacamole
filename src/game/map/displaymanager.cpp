#include "displaymanager.h"
#include "mapobject.h"
#include "tile.h"
#include "../entity/entity.h"

namespace game
{
namespace map
{

inline int compareTileAndEntityDepth(const Tile* tile, const entity::Entity* entity)
{
	if (tile->getDepth() < entity->getDepth())
	{
		return -1;
	}
	else if (tile->getMapObjectZ() < entity->getMapObjectZ() + 0.01f)
	{
		return -1;
	}
	/*
	if (tile->getDepth() < entity->getDepth() || tile->getMapObjectZ() < entity->getMapObjectZ() + 0.01f)
	{
		return -1;
	}
	*/
	return 1;
}

inline int compareMapObjectsDepth(const MapObject* a, const MapObject* b)
{
	const bool aIsTile = a->isTile();
	const bool bIsTile = b->isTile();

	if (aIsTile == bIsTile)
	{
		return flat::sign(a->getDepth() - b->getDepth());
	}
	else if (aIsTile)
	{
		FLAT_ASSERT(aIsTile && !bIsTile);
		const Tile* aTile = static_cast<const Tile*>(a);
		const entity::Entity* bEntity = static_cast<const entity::Entity*>(b);
		return compareTileAndEntityDepth(aTile, bEntity);
	}
	else
	{
		FLAT_ASSERT(!aIsTile && bIsTile);
		const entity::Entity* aEntity = static_cast<const entity::Entity*>(a);
		const Tile* bTile = static_cast<const Tile*>(b);
		return -compareTileAndEntityDepth(bTile, aEntity);
	}
}

inline bool isLessDeep(const MapObject* a, const MapObject* b)
{
	const int cmp = compareMapObjectsDepth(a, b);
	if (cmp == 0)
	{
		return a->getTextureHash() < b->getTextureHash();
	}
	else
	{
		return cmp < 0;
	}
}

void DisplayManager::sortByDepthAndDraw(const flat::render::RenderSettings& renderSettings, const flat::Matrix4& viewMatrix)
{
	std::sort(std::begin(m_objects), std::end(m_objects), &isLessDeep);
	FLAT_ASSERT_MSG(std::is_sorted(std::begin(m_objects), std::end(m_objects), &isLessDeep), "compare function provides a wrong order relation");

	/*
	for (const MapObject* mapObject : m_objects)
	{
		mapObject->getSprite().draw(renderSettings, viewMatrix);
	}
	*/

	flat::render::SpriteBatch spriteBatch;
	std::vector<const MapObject*>::iterator it = m_objects.begin();
	std::vector<const MapObject*>::iterator end = m_objects.end();
	while (it != m_objects.end())
	{
		spriteBatch.clear();

#if 0
		const MapObject* mapObject = *it;
		std::vector<const MapObject*>::iterator it2 = std::upper_bound(it, end, mapObject, &isLessDeep);
		while (it != it2)
		{
			mapObject = *it;
			spriteBatch.add(mapObject->getSprite());
			++it;
		}
#else
		std::vector<const MapObject*>::iterator it2 = it;

		const std::uint32_t currentHash = (*it)->getTextureHash();
		while (it2 != m_objects.end() && (*it2)->getTextureHash() == currentHash)
		{
			++it2;
		}
		while (it != it2)
		{
			spriteBatch.add((*it)->getSprite());
			++it;
		}
#endif

		spriteBatch.draw(renderSettings, viewMatrix);
	}
}

} // map
} // game



