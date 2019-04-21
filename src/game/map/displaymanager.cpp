#include <execution>

#include "displaymanager.h"
#include "mapobject.h"
#include "map.h"
#include "tile.h"
#include "../entity/entity.h"
#include "../game.h"

#define DEBUG_DRAW 0

namespace game
{
namespace map
{

DisplayManager::DisplayManager()
#ifdef FLAT_DEBUG
	: m_numOpaqueObjects(0)
	, m_numOpaqueDrawCalls(0)
	, m_numTransparentObjects(0)
	, m_numTransparentDrawCalls(0)
#endif
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

void DisplayManager::addEntity(const entity::Entity* entity)
{
	if (entity->hasSprite())
	{
		entity->updateRenderHash();
		int cellIndex = m_entityQuadtree->addObject(entity);
		m_entityCellIndices[entity] = cellIndex;
	}
}

void DisplayManager::removeEntity(const entity::Entity* entity)
{
	if (entity->hasSprite())
	{
		int cellIndex = m_entityCellIndices.at(entity);
		m_entityQuadtree->removeObject(entity, cellIndex);
		m_entityCellIndices.erase(entity);
	}
}

void DisplayManager::updateEntity(const entity::Entity* entity)
{
	if (entity->hasSprite())
	{
		int cellIndex = m_entityCellIndices.at(entity);
		int newCellIndex = m_entityQuadtree->updateObject(entity, cellIndex);
		if (cellIndex != newCellIndex)
		{
			m_entityCellIndices[entity] = newCellIndex;
		}
	}
}

void DisplayManager::addTerrainObject(const MapObject* mapObject)
{
	FLAT_ASSERT(!mapObject->isEntity());
	mapObject->updateRenderHash();
	int cellIndex = m_terrainQuadtree->addObject(mapObject);
	m_TerrainObjectCellIndices[mapObject] = cellIndex;
}

void DisplayManager::removeTerrainObject(const MapObject* mapObject)
{
	FLAT_ASSERT(!mapObject->isEntity());
	int cellIndex = m_TerrainObjectCellIndices[mapObject];
	m_terrainQuadtree->removeObject(mapObject, cellIndex);
	m_TerrainObjectCellIndices.erase(mapObject);
}

void DisplayManager::updateTerrainObject(const MapObject* mapObject)
{
	FLAT_ASSERT(!mapObject->isEntity());
	int cellIndex = m_TerrainObjectCellIndices[mapObject];
	int newCellIndex = m_terrainQuadtree->updateObject(mapObject, cellIndex);
	if (cellIndex != newCellIndex)
	{
		m_TerrainObjectCellIndices[mapObject] = newCellIndex;
	}
}

void DisplayManager::sortAndDraw(Game& game, const flat::video::View& view)
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

	sortObjects(objects);

	// set the object depth according to its index
	const int numObjects = static_cast<int>(objects.size());
	for (int i = 0; i < numObjects; ++i)
	{
		const MapObject* mapObject = objects[i];
		const float depth = static_cast<float>(numObjects - i) / numObjects;
		const_cast<flat::render::BaseSprite&>(mapObject->getSprite()).setDepth(depth);
	}

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	{
		std::vector<const MapObject*> opaqueObjects = objects;
		opaqueObjects.erase(std::remove_if(
			opaqueObjects.begin(),
			opaqueObjects.end(),
			[](const MapObject* a) { return a->getSprite().requiresAlphaBlending(); }
		), opaqueObjects.end());

		std::sort(
			std::execution::par,
			opaqueObjects.begin(),
			opaqueObjects.end(),
			[](const MapObject* a, const MapObject* b)
			{
				return a->getRenderHash() < b->getRenderHash();
			}
		);

		glDisable(GL_BLEND);

#ifdef FLAT_DEBUG
		drawBatches(game, view, opaqueObjects, m_numOpaqueObjects, m_numOpaqueDrawCalls);
#else
		drawBatches(game, view, opaqueObjects);
#endif
	}

	{
		std::vector<const MapObject*> transparentObjects = objects;
		transparentObjects.erase(std::remove_if(
			transparentObjects.begin(),
			transparentObjects.end(),
			[](const MapObject* a) { return !a->getSprite().requiresAlphaBlending(); }
		), transparentObjects.end());

		glEnable(GL_BLEND);

#ifdef FLAT_DEBUG
		drawBatches(game, view, transparentObjects, m_numTransparentObjects, m_numTransparentDrawCalls);
#else
		drawBatches(game, view, transparentObjects);
#endif
	}

	glDisable(GL_DEPTH_TEST);
}

const MapObject* DisplayManager::getObjectAtPosition(const flat::Vector2& position) const
{
	std::vector<const MapObject*> objects;
	objects.reserve(16);
	m_entityQuadtree->getObjects(position, objects);
	m_terrainQuadtree->getObjects(position, objects);
	sortObjects(objects);
	
	// look for a visible pixel in the objects' sprite
	const MapObject* objectAtPosition = nullptr;
	for (int i = static_cast<int>(objects.size()) - 1; objectAtPosition == nullptr && i >= 0; --i)
	{
		const MapObject* object = objects[i];
		const flat::render::BaseSprite& sprite = object->getSprite();

		flat::video::Color color;
		sprite.getPixel(position, color);
		if (color.a > 0.5f)
		{
			objectAtPosition = object;
		}
	}

	return objectAtPosition;
}

void DisplayManager::getEntitiesInAABB(const flat::AABB2& aabb, std::vector<const MapObject*>& entities) const
{
	m_entityQuadtree->getObjects(aabb, entities);
}

const Tile* DisplayManager::getTileAtPosition(const flat::Vector2& position) const
{
	std::vector<const MapObject*> objects;
	objects.reserve(16);
	m_terrainQuadtree->getObjects(position, objects);
	sortObjects(objects);

	// look for a visible pixel in the objects' sprite
	const MapObject* objectAtPosition = nullptr;
	for (int i = static_cast<int>(objects.size()) - 1; objectAtPosition == nullptr && i >= 0; --i)
	{
		const MapObject* object = objects[i];
		if (object->isTile())
		{
			const flat::render::BaseSprite& sprite = object->getSprite();

			flat::video::Color color;
			sprite.getPixel(position, color);
			if (color.a > 0.5f)
			{
				objectAtPosition = object;
			}
		}
	}

	return static_cast<const Tile*>(objectAtPosition);
}

#ifdef FLAT_DEBUG
const flat::AABB2& DisplayManager::getEntityCellAABB(const entity::Entity* entity) const
{
	int cellIndex = m_entityCellIndices.at(entity);
	const EntityQuadTree::Cell& cell = m_entityQuadtree->getCell(cellIndex);
	return cell.getAABB();
}
#endif

// Object sorting

inline bool locSortByDepth(const MapObject* a, const MapObject* b)
{
	const flat::Vector3 aCenter = a->getWorldSpaceAABB().getCenter();
	const flat::Vector3 bCenter = b->getWorldSpaceAABB().getCenter();
	const float aDepth = aCenter.x + aCenter.y;
	const float bDepth = bCenter.x + bCenter.y;
	if (aDepth == bDepth)
	{
		if (aCenter.z == bCenter.z)
		{
			if (a->getRenderHash() == b->getRenderHash())
			{
				return aCenter.x < bCenter.x;
			}
			return a->getRenderHash() < b->getRenderHash();
		}
		return aCenter.z < bCenter.z;
	}
	return aDepth < bDepth;
}

inline bool locSortByAltitude(const MapObject* a, const MapObject* b)
{
	const flat::AABB3& aAABB = a->getWorldSpaceAABB();
	const flat::AABB3& bAABB = b->getWorldSpaceAABB();
	const float aDepth = aAABB.min.z + 0.01f;
	const float bDepth = bAABB.max.z;
	if (aDepth == bDepth)
	{
		if (a->getRenderHash() == b->getRenderHash())
		{
			return aAABB.getCenter().x < bAABB.getCenter().x;
		}
		return a->getRenderHash() < b->getRenderHash();
	}
	return aDepth < bDepth;
}

inline bool spritesOverlap(const MapObject* a, const MapObject* b)
{
	return flat::AABB2::overlap(a->getAABB(), b->getAABB());
}

void DisplayManager::sortObjects(std::vector<const MapObject*>& objects)
{
	std::sort(std::execution::par, objects.begin(), objects.end(), locSortByDepth);
	FLAT_ASSERT(std::is_sorted(objects.begin(), objects.end(), locSortByDepth));

	{
#if DEBUG_DRAW
		for (const MapObject* o : objects)
		{
			const_cast<MapObject*>(o)->getSprite().setColor(flat::video::Color::WHITE);

#ifdef FLAT_DEBUG
			const flat::AABB2& currentSpriteAABB = o->getAABB();
			flat::AABB2 desiredSpriteAABB;
			o->getSprite().getAABB(desiredSpriteAABB);
			FLAT_ASSERT(desiredSpriteAABB == currentSpriteAABB);
#endif
		}
#endif

		// move entities in front of overlapping tiles if necessary
		const int size = static_cast<int>(objects.size());
		int numSwaps = 0;
		for (int i = 0; i < size; ++i)
		{
			const MapObject* mapObject = objects[i];
			if (mapObject->isEntity())
			{
				const flat::Vector3 aCenter = mapObject->getWorldSpaceAABB().getCenter();
				const float aDepth = aCenter.x + aCenter.y;
				int k = 0;
				for (int j = i + 1; j < size; ++j)
				{
					const MapObject* mapObject2 = objects[j];
					const flat::Vector3 bCenter = mapObject2->getWorldSpaceAABB().getCenter();
					const float bDepth = bCenter.x + bCenter.y;

					if (bDepth > aDepth + 1.f)
						break;

					if (spritesOverlap(mapObject, mapObject2)
						&& (mapObject2->isTile() && !locSortByAltitude(mapObject, mapObject2)
							|| mapObject2->isEntity() && !locSortByDepth(mapObject, mapObject2)))
					{
#if DEBUG_DRAW
						const_cast<MapObject*>(mapObject)->getSprite().setColor(flat::video::Color::RED);
						const_cast<MapObject*>(mapObject2)->getSprite().setColor(flat::video::Color::BLUE);
#endif
						if (mapObject2->isTile() || aDepth != bDepth)
						{
							k = j;
						}
					}
				}
				if (k != 0)
				{
					/*while (k < size - 1 && objects[k + 1]->isEntity() && locSortByDepth(mapObject, objects[k + 1]))
					{
						++k;
					}*/

#if DEBUG_DRAW
					const_cast<MapObject*>(objects[k])->getSprite().setColor(flat::video::Color::GREEN);
#endif
					std::rotate(objects.begin() + i, objects.begin() + i + 1, objects.begin() + k + 1);
					--i; // avoids to skip the 1st shifted element
					++numSwaps;
				}
			}
		}

#if DEBUG_DRAW
		std::cout << "swaps: " << numSwaps << std::endl;
#endif
	}
}

#ifdef FLAT_DEBUG
void DisplayManager::drawBatches(Game& game, const flat::video::View& view, const std::vector<const MapObject*>& objects, size_t& numObjects, size_t& numDrawCalls)
#else
void DisplayManager::drawBatches(Game& game, const flat::video::View& view, const std::vector<const MapObject*>& objects)
#endif
{

#ifdef FLAT_DEBUG
	numObjects = objects.size();
	numDrawCalls = 0;
#endif


	flat::video::Window& window = *game.video->window;

	flat::render::SpriteBatch* spriteBatch = m_spriteBatch.get();
	const flat::Matrix4& viewMatrix = view.getViewProjectionMatrix();

	std::vector<const MapObject*>::const_iterator it = objects.begin();
	std::vector<const MapObject*>::const_iterator end = objects.end();
	while (it != end)
	{
		spriteBatch->clear();

		std::vector<const MapObject*>::const_iterator it2 = it;
		while (it2 != end && (*it2)->getRenderHash() == (*it)->getRenderHash())
		{
			spriteBatch->add((*it2)->getSprite());
			++it2;
		}

		const flat::render::ProgramSettings& programSettings = (*it)->getProgramSettings();
		it = it2;

		programSettings.program.use(window);
		programSettings.settings.viewProjectionMatrixUniform.set(view.getViewProjectionMatrix());
		spriteBatch->draw(programSettings.settings, viewMatrix);

#ifdef FLAT_DEBUG
		++numDrawCalls;
#endif
	}
}

#undef DEBUG_DRAW

} // map
} // game



