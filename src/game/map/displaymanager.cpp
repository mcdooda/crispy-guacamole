#include "displaymanager.h"
#include "mapobject.h"
#include "map.h"
#include "tile.h"
#include "../entity/entity.h"

#define DEBUG_DRAW 0

namespace game
{
namespace map
{

DisplayManager::DisplayManager()
{
	m_spriteBatch = std::make_unique<flat::render::SpriteBatch>();
}

inline bool sortMapObjects(const MapObject* a, const MapObject* b)
{
	const flat::Vector3 aCenter = a->getAABB().getCenter();
	const flat::Vector3 bCenter = b->getAABB().getCenter();
	const float aDepth = aCenter.x + aCenter.y;
	const float bDepth = bCenter.x + bCenter.y;
	if (aDepth == bDepth)
	{
		if (aCenter.z == bCenter.z)
		{
			if (a->getTextureHash() == b->getTextureHash())
			{
				return aCenter.x < bCenter.x;
			}
			return a->getTextureHash() < b->getTextureHash();
		}
		return aCenter.z < bCenter.z;
	}
	return aDepth < bDepth;
}

inline bool sortMapObjects2(const MapObject* a, const MapObject* b)
{
	const float aDepth = a->getAABB().min.z + 0.01f;
	const float bDepth = b->getAABB().max.z;
	if (aDepth == bDepth)
	{
		if (a->getTextureHash() == b->getTextureHash())
		{
			return a->getAABB().getCenter().x < b->getAABB().getCenter().x;
		}
		return a->getTextureHash() < b->getTextureHash();
	}
	return aDepth < bDepth;
}

inline bool spritesOverlap(const MapObject* a, const MapObject* b)
{
	return a->getSprite().overlaps(b->getSprite());
}

void DisplayManager::add(const MapObject* mapObject)
{
	FLAT_ASSERT(mapObject->getTextureHash() != 0);
	m_objects.push_back(mapObject);
}

void DisplayManager::sortByDepthAndDraw(const flat::render::RenderSettings& renderSettings, const flat::Matrix4& viewMatrix)
{
	std::sort(m_objects.begin(), m_objects.end(), sortMapObjects);
	FLAT_ASSERT(std::is_sorted(m_objects.begin(), m_objects.end(), sortMapObjects));

	{
#if DEBUG_DRAW
		for (const MapObject* o : m_objects)
		{
			const_cast<flat::render::Sprite&>(o->getSprite()).setColor(flat::video::Color::WHITE);
		}
#endif

		const int size = static_cast<int>(m_objects.size());
		int numSwaps = 0;
		for (int i = 0; i < size; ++i)
		{
			const MapObject* mapObject = m_objects[i];
			const flat::Vector3 aCenter = mapObject->getAABB().getCenter();
			const float aDepth = aCenter.x + aCenter.y;
			if (mapObject->isEntity())
			{
				int k = 0;
				for (int j = i + 1; j < size; ++j)
				{
					const MapObject* mapObject2 = m_objects[j];
					const flat::Vector3 bCenter = mapObject2->getAABB().getCenter();
					const float bDepth = bCenter.x + bCenter.y;

					if (bDepth > aDepth + 1.f)
						break;

					if (mapObject2->isTile() && spritesOverlap(mapObject, mapObject2) && !sortMapObjects2(mapObject, mapObject2))
					{
#if DEBUG_DRAW
						const_cast<flat::render::Sprite&>(mapObject->getSprite()).setColor(flat::video::Color::RED);
						const_cast<flat::render::Sprite&>(mapObject2->getSprite()).setColor(flat::video::Color::BLUE);
#endif
						if (k == 0 || !sortMapObjects(m_objects[j], m_objects[k]))
						{
							k = j;
						}
					}
				}
				if (k != 0)
				{
					while (k < size - 1 && m_objects[k + 1]->isEntity() && sortMapObjects2(mapObject, m_objects[k + 1]))
					{
						++k;
					}

#if DEBUG_DRAW
					const_cast<flat::render::Sprite&>(m_objects[k]->getSprite()).setColor(flat::video::Color::GREEN);
#endif
					for (int l = i; l < k; ++l)
					{
						m_objects[l] = m_objects[l + 1];
					}
					m_objects[k] = mapObject;
					--i; // avoids to skip the 1st shifted element
					++numSwaps;
				}
			}
		}

#if DEBUG_DRAW
		std::cout << "numSwaps = " << numSwaps << std::endl;
#endif
	}


	{
#if DEBUG_DRAW
		int numDrawCalls = 0;
		int numSprites = 0;
#endif

		flat::render::SpriteBatch* spriteBatch = m_spriteBatch.get();

		std::vector<const MapObject*>::iterator it = m_objects.begin();
		std::vector<const MapObject*>::iterator end = m_objects.end();
		while (it != end)
		{
			spriteBatch->clear();

			std::vector<const MapObject*>::iterator it2 = it;
			while (it2 != end && (*it2)->getTextureHash() == (*it)->getTextureHash())
			{
				spriteBatch->add((*it2)->getSprite());
#if DEBUG_DRAW
				++numSprites;
#endif
				++it2;
			}
			it = it2;

			spriteBatch->draw(renderSettings, viewMatrix);
#if DEBUG_DRAW
			++numDrawCalls;
#endif
		}

#if DEBUG_DRAW
		std::cout << "numDrawCalls = " << numDrawCalls << " / numSprites = " << numSprites << std::endl;
#endif
	}
}

#undef DEBUG_DRAW

} // map
} // game



