#include "displaymanager.h"
#include "mapobject.h"
#include "map.h"

namespace game
{
namespace map
{

DisplayManager::DisplayManager()
{
	m_spriteBatch = std::make_unique<flat::render::SpriteBatch>();
}

struct Hexagon
{
	float xmin;
	float xmax;
	float ymin;
	float ymax;
	float hmin;
	float hmax;
};

inline void getHexagonFromAABB(const flat::Matrix3& mapTransform, const flat::AABB3& aabb, Hexagon& hexagon)
{
	hexagon.xmin = aabb.max.x;
	hexagon.xmax; // TODO
	hexagon.ymin = aabb.max.y;
	hexagon.ymax; // TODO
	hexagon.hmin = aabb.min.y - aabb.max.x;
	hexagon.hmax = aabb.min.x - aabb.max.y;
}

inline bool hexagonsOverlap(const flat::Matrix3& mapTransform, const flat::AABB3& aAABB, const flat::AABB3& bAABB)
{
	Hexagon aHexagon;
	getHexagonFromAABB(mapTransform, aAABB, aHexagon);

	Hexagon bHexagon;
	getHexagonFromAABB(mapTransform, bAABB, bHexagon);

	return !(aHexagon.xmin >= bHexagon.xmax || bHexagon.xmin >= aHexagon.xmax) &&
	       !(aHexagon.ymin >= bHexagon.ymax || bHexagon.ymin >= aHexagon.ymax) &&
	       !(aHexagon.hmin >= bHexagon.hmax || bHexagon.hmin >= aHexagon.hmax);
}

class SortMapObjects
{
	public:
		SortMapObjects(const flat::Matrix3& mapTransform) :
			m_mapTransform(mapTransform)
		{}

		inline bool operator()(const MapObject* a, const MapObject* b)
		{
			const flat::Vector3 aCenter = a->getAABB().getCenter();
			const flat::Vector3 bCenter = b->getAABB().getCenter();
			const float aDepth = aCenter.x + aCenter.y;
			const float bDepth = bCenter.x + bCenter.y;
			if (aDepth == bDepth)
			{
				return a->getTextureHash() < b->getTextureHash();
			}
			return aDepth < bDepth;

			/*
			const flat::AABB3& aAABB = a->getAABB();
			const flat::AABB3& bAABB = b->getAABB();

			if (!hexagonsOverlap(m_mapTransform, aAABB, bAABB))
			{
				return a->getTextureHash() < b->getTextureHash();
			}

			if (aAABB.min.x >= bAABB.max.x)
				return false;
			else if (bAABB.min.x >= aAABB.max.x)
				return true;

			if (aAABB.min.y >= bAABB.max.y)
				return false;
			else if (bAABB.min.y >= aAABB.max.y)
				return true;

			if (aAABB.min.z >= bAABB.max.z)
				return false;
			else if (bAABB.min.z >= aAABB.max.z)
				return true;

			return a->getTextureHash() < b->getTextureHash();
			*/
		}

	private:
		const flat::Matrix3& m_mapTransform;
};

void DisplayManager::sortByDepthAndDraw(const flat::render::RenderSettings& renderSettings, const flat::Matrix4& viewMatrix)
{
	const flat::Matrix3& mapTransform = m_map->getTransform();
	SortMapObjects sorter(mapTransform);
	std::sort(std::begin(m_objects), std::end(m_objects), sorter);
	FLAT_ASSERT_MSG(std::is_sorted(std::begin(m_objects), std::end(m_objects), sorter), "Sort function is not coherent");

	/*for (const MapObject* mapObject : m_objects)
	{
		mapObject->getSprite().draw(renderSettings, viewMatrix);
	}*/

	flat::render::SpriteBatch* spriteBatch = m_spriteBatch.get();

	std::vector<const MapObject*>::iterator it = m_objects.begin();
	std::vector<const MapObject*>::iterator end = m_objects.end();
	while (it != m_objects.end())
	{
		spriteBatch->clear();

		const MapObject* mapObject = *it;
		std::vector<const MapObject*>::iterator it2 = std::upper_bound(it, end, mapObject, sorter);
		while (it != it2)
		{
			mapObject = *it;
			spriteBatch->add(mapObject->getSprite());
			++it;
		}

		spriteBatch->draw(renderSettings, viewMatrix);
	}
}

} // map
} // game



