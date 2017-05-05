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
	const float quadtreeSize = 16384.f;
	m_entityQuadtree = std::make_unique<EntityQuadTree>(
		flat::AABB2(
			flat::Vector2(-quadtreeSize / 2.f, -quadtreeSize / 2.f),
			flat::Vector2( quadtreeSize / 2.f,  quadtreeSize / 2.f)
		)
	);
	m_terrainQuadtree = std::make_unique<TerrainQuadTree>(
		flat::AABB2(
			flat::Vector2(-quadtreeSize / 2.f, -quadtreeSize / 2.f),
			flat::Vector2(quadtreeSize / 2.f, quadtreeSize / 2.f)
		)
	);
}

inline bool sortMapObjects(const MapObject* a, const MapObject* b)
{
	const flat::Vector3 aCenter = a->getWorldSpaceAABB().getCenter();
	const flat::Vector3 bCenter = b->getWorldSpaceAABB().getCenter();
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
	const float aDepth = a->getWorldSpaceAABB().min.z + 0.01f;
	const float bDepth = b->getWorldSpaceAABB().max.z;
	if (aDepth == bDepth)
	{
		if (a->getTextureHash() == b->getTextureHash())
		{
			return a->getWorldSpaceAABB().getCenter().x < b->getWorldSpaceAABB().getCenter().x;
		}
		return a->getTextureHash() < b->getTextureHash();
	}
	return aDepth < bDepth;
}

inline bool spritesOverlap(const MapObject* a, const MapObject* b)
{
	return a->getSprite().overlaps(b->getSprite());
}

void DisplayManager::updateEntities()
{
	m_entityQuadtree->updateAllObjects(m_entityCellIndices);
}

void DisplayManager::addEntity(const MapObject* mapObject)
{
	FLAT_ASSERT(mapObject->getTextureHash() != 0);
	FLAT_ASSERT(mapObject->isEntity());
	int cellIndex = m_entityQuadtree->addObject(mapObject);
	m_entityCellIndices[mapObject] = cellIndex;
}

void DisplayManager::removeEntity(const MapObject* mapObject)
{
	FLAT_ASSERT(mapObject->getTextureHash() != 0);
	FLAT_ASSERT(mapObject->isEntity());
	int cellIndex = m_entityCellIndices[mapObject];
	m_entityQuadtree->removeObject(mapObject, cellIndex);
	m_entityCellIndices.erase(mapObject);
}

void DisplayManager::updateEntity(const MapObject* mapObject)
{
	FLAT_ASSERT(mapObject->getTextureHash() != 0);
	FLAT_ASSERT(mapObject->isEntity());
	int cellIndex = m_entityCellIndices[mapObject];
	int newCellIndex = m_entityQuadtree->updateObject(mapObject, cellIndex);
	if (cellIndex != newCellIndex)
	{
		m_entityCellIndices[mapObject] = newCellIndex;
	}
}

void DisplayManager::addTerrainObject(const MapObject * mapObject)
{
	FLAT_ASSERT(mapObject->getTextureHash() != 0);
	FLAT_ASSERT(!mapObject->isEntity());
	m_terrainQuadtree->addObject(mapObject);
}

void DisplayManager::removeTerrainObject(const MapObject * mapObject)
{
	FLAT_ASSERT(mapObject->getTextureHash() != 0);
	FLAT_ASSERT(!mapObject->isEntity());
	m_terrainQuadtree->removeObject(mapObject);
}

void DisplayManager::sortByDepthAndDraw(const flat::render::RenderSettings& renderSettings, const flat::video::View& view)
{
	flat::AABB2 screenAABB;
	view.getScreenAABB(screenAABB);

	std::vector<const MapObject*> objects;
	objects.reserve(128);

	m_entityQuadtree->getObjects(screenAABB, objects);
#ifdef DEBUG_DRAW
	int numEntities = static_cast<int>(objects.size());
#endif
	m_terrainQuadtree->getObjects(screenAABB, objects);
#ifdef DEBUG_DRAW
	int numTerrainObjects = static_cast<int>(objects.size()) - numEntities;
#endif

	std::sort(objects.begin(), objects.end(), sortMapObjects);
	FLAT_ASSERT(std::is_sorted(objects.begin(), objects.end(), sortMapObjects));

	{
#if DEBUG_DRAW
		for (const MapObject* o : objects)
		{
			const_cast<flat::render::Sprite&>(o->getSprite()).setColor(flat::video::Color::WHITE);
		}
#endif

		const int size = static_cast<int>(objects.size());
		int numSwaps = 0;
		for (int i = 0; i < size; ++i)
		{
			const MapObject* mapObject = objects[i];
			const flat::Vector3 aCenter = mapObject->getWorldSpaceAABB().getCenter();
			const float aDepth = aCenter.x + aCenter.y;
			if (mapObject->isEntity())
			{
				int k = 0;
				for (int j = i + 1; j < size; ++j)
				{
					const MapObject* mapObject2 = objects[j];
					const flat::Vector3 bCenter = mapObject2->getWorldSpaceAABB().getCenter();
					const float bDepth = bCenter.x + bCenter.y;

					if (bDepth > aDepth + 1.f)
						break;

					if (mapObject2->isTile() && spritesOverlap(mapObject, mapObject2) && !sortMapObjects2(mapObject, mapObject2))
					{
#if DEBUG_DRAW
						const_cast<flat::render::Sprite&>(mapObject->getSprite()).setColor(flat::video::Color::RED);
						const_cast<flat::render::Sprite&>(mapObject2->getSprite()).setColor(flat::video::Color::BLUE);
#endif
						if (k == 0 || !sortMapObjects(objects[j], objects[k]))
						{
							k = j;
						}
					}
				}
				if (k != 0)
				{
					while (k < size - 1 && objects[k + 1]->isEntity() && sortMapObjects2(mapObject, objects[k + 1]))
					{
						++k;
					}

#if DEBUG_DRAW
					const_cast<flat::render::Sprite&>(objects[k]->getSprite()).setColor(flat::video::Color::GREEN);
#endif
					for (int l = i; l < k; ++l)
					{
						objects[l] = objects[l + 1];
					}
					objects[k] = mapObject;
					--i; // avoids to skip the 1st shifted element
					++numSwaps;
				}
			}
		}

#if DEBUG_DRAW
		std::cout << "swaps: " << numSwaps << std::endl;
#endif
	}


	{
#if DEBUG_DRAW
		int numDrawCalls = 0;
#endif

		flat::render::SpriteBatch* spriteBatch = m_spriteBatch.get();
		const flat::Matrix4& viewMatrix = view.getViewProjectionMatrix();

		std::vector<const MapObject*>::iterator it = objects.begin();
		std::vector<const MapObject*>::iterator end = objects.end();
		while (it != end)
		{
			spriteBatch->clear();

			std::vector<const MapObject*>::iterator it2 = it;
			while (it2 != end && (*it2)->getTextureHash() == (*it)->getTextureHash())
			{
				spriteBatch->add((*it2)->getSprite());
				++it2;
			}
			it = it2;

			spriteBatch->draw(renderSettings, viewMatrix);
#if DEBUG_DRAW
			++numDrawCalls;
#endif
		}

#if DEBUG_DRAW
		std::cout << "draw calls: " << numDrawCalls << std::endl
			<< "terrain sprites: " << numTerrainObjects << std::endl
			<< "entity sprites: " << numEntities << std::endl << std::endl;
#endif
	}
}

#undef DEBUG_DRAW

} // map
} // game



