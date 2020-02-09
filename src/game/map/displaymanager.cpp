#include <execution>

#include "map/displaymanager.h"
#include "map/mapobject.h"
#include "map/tile.h"
#include "map/prop.h"
#include "map/fog/fog.h"

#include "entity/entity.h"

#include "game.h"

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
	const flat::AABB2 quadtreeAABB(flat::Vector2(-quadtreeSize / 2.f, -quadtreeSize / 2.f), flat::Vector2(quadtreeSize / 2.f, quadtreeSize / 2.f));
	m_entityQuadtree = std::make_unique<EntityQuadTree>(quadtreeAABB);
	m_tileQuadtree = std::make_unique<TileQuadTree>(quadtreeAABB);
	m_propQuadtree = std::make_unique<PropQuadTree>(quadtreeAABB);
}

void DisplayManager::clear()
{
	//m_entityQuadtree->clear();
	//m_entityCellIndices.clear();

	m_tileQuadtree->clear();
	m_tileCellIndices.clear();

	m_propQuadtree->clear();
	m_propCellIndices.clear();
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

void DisplayManager::addTile(TileIndex tileIndex, const Tile* tile)
{
	tile->updateRenderHash();
	int cellIndex = m_tileQuadtree->addObject(tileIndex, tile->getAABB());
	m_tileCellIndices[tileIndex] = cellIndex;
}

void DisplayManager::removeTile(TileIndex tileIndex)
{
	int cellIndex = m_tileCellIndices[tileIndex];
	m_tileQuadtree->removeObject(tileIndex, cellIndex);
	m_tileCellIndices.erase(tileIndex);
}

void DisplayManager::updateTile(TileIndex tileIndex, const Tile* tile)
{
	int cellIndex = m_tileCellIndices[tileIndex];
	int newCellIndex = m_tileQuadtree->updateObject(tileIndex, cellIndex, tile->getAABB());
	if (cellIndex != newCellIndex)
	{
		m_tileCellIndices[tileIndex] = newCellIndex;
	}
}

void DisplayManager::moveTileIndex(TileIndex fromIndex, TileIndex toIndex)
{
	int cellIndex = m_tileCellIndices[fromIndex];
	m_tileQuadtree->replaceObject(fromIndex, toIndex, cellIndex);
	m_tileCellIndices[toIndex] = cellIndex;
	m_tileCellIndices.erase(fromIndex);
}

void DisplayManager::addProp(PropIndex propIndex, const Prop* prop)
{
	prop->updateRenderHash();
	int cellIndex = m_propQuadtree->addObject(propIndex, prop->getAABB());
	m_propCellIndices[propIndex] = cellIndex;
}

void DisplayManager::removeProp(PropIndex propIndex)
{
	int cellIndex = m_propCellIndices[propIndex];
	m_propQuadtree->removeObject(propIndex, cellIndex);
	m_propCellIndices.erase(propIndex);
}

void DisplayManager::updateProp(PropIndex propIndex, const Prop* prop)
{
	int cellIndex = m_propCellIndices[propIndex];
	int newCellIndex = m_propQuadtree->updateObject(propIndex, cellIndex, prop->getAABB());
	if (cellIndex != newCellIndex)
	{
		m_propCellIndices[propIndex] = newCellIndex;
	}
}

void DisplayManager::movePropIndex(PropIndex fromIndex, PropIndex toIndex)
{
	int cellIndex = m_propCellIndices[fromIndex];
	m_propQuadtree->replaceObject(fromIndex, toIndex, cellIndex);
	m_propCellIndices[toIndex] = cellIndex;
	m_propCellIndices.erase(fromIndex);
}

void DisplayManager::addTemporaryObject(const MapObject* mapObject)
{
	m_temporaryObjects.push_back(mapObject);
}

void DisplayManager::sortAndDraw(Game& game, const map::fog::Fog& fog, const flat::video::View& view)
{
	flat::AABB2 screenAABB;
	view.getScreenAABB(screenAABB);

	// entities
	std::vector<const entity::Entity*> entities;
	entities.reserve(1024);
	{
		FLAT_PROFILE("Display Manager Get Entities");
		m_entityQuadtree->getObjects(screenAABB, entities);
	}
	int numEntities = static_cast<int>(entities.size());
	for (int i = 0, e = numEntities; i < e; ++i)
	{
		const entity::Entity* entity = entities[i];
		const TileIndex tileIndex = entity->getTileIndexFromPosition();
		if (!fog.isTileDiscovered(tileIndex))
		{
			entities[i] = nullptr;
			--numEntities;
		}
	}

	// tiles
	std::vector<TileIndex> tileIndices;
	tileIndices.reserve(1024);
	{
		FLAT_PROFILE("Display Manager Get Tiles");
		m_tileQuadtree->getObjects(screenAABB, tileIndices);
	}
	std::vector<const Tile*> tiles;
	fog.getTilesFromIndices(tileIndices, tiles);

	// props
	std::vector<PropIndex> propIndices;
	propIndices.reserve(1024);
	{
		FLAT_PROFILE("Display Manager Get Props");
		m_propQuadtree->getObjects(screenAABB, propIndices);
	}
	std::vector<const Prop*> props;
	fog.getPropsFromIndices(propIndices, props);

	// put everything into a single vector
	std::vector<const MapObject*> objects;
	objects.reserve(tiles.size() + props.size() + m_temporaryObjects.size() + numEntities);

	// tiles
	for (const Tile* tile : tiles)
	{
		objects.push_back(tile);
	}

	// props
	for (const Prop* prop : props)
	{
		objects.push_back(prop);
	}

	// temporary objects
	objects.insert(objects.end(), m_temporaryObjects.begin(), m_temporaryObjects.end());
	m_temporaryObjects.clear();

	// entities and attached sprites
	for (const entity::Entity* entity : entities)
	{
		if (entity != nullptr)
		{
			objects.push_back(entity);
			entity->pushAttachedSprites(objects);
		}
	}

	{
		FLAT_PROFILE("Display Manager Sort Objects");
		sortObjects(objects);
	}

	{
		FLAT_PROFILE("Display Manager Set Objects Depth");

		// set the object depth according to its index
		const int numObjects = static_cast<int>(objects.size());
		const float numObjectsPlusOneInverse = 1.f / (static_cast<float>(numObjects) + 1.f);
		for (int i = 0; i < numObjects; ++i)
		{
			const MapObject* mapObject = objects[i];
			const float depth = static_cast<float>(numObjects - i) * numObjectsPlusOneInverse;
			const_cast<flat::render::BaseSprite&>(mapObject->getSprite()).setDepth(depth);
		}
	}

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	{
		FLAT_PROFILE("Display Manager Draw Opaque Objects");

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
		FLAT_PROFILE("Display Manager Draw Transparent Objects");

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

const MapObject* DisplayManager::getObjectAtPosition(const map::fog::Fog& fog, const flat::Vector2& position) const
{
	std::vector<const MapObject*> objects;
	objects.reserve(16);
	m_entityQuadtree->getObjects(position, objects);

	std::vector<TileIndex> tileIndices;
	tileIndices.reserve(8);
	m_tileQuadtree->getObjects(position, tileIndices);
	std::vector<const Tile*> tiles;
	fog.getTilesFromIndices(tileIndices, tiles);

	std::vector<PropIndex> propIndices;
	propIndices.reserve(8);
	m_propQuadtree->getObjects(position, propIndices);
	std::vector<const Prop*> props;
	fog.getPropsFromIndices(propIndices, props);

	objects.reserve(objects.size() + tiles.size() + props.size());
	objects.insert(objects.end(), tiles.begin(), tiles.end());
	objects.insert(objects.end(), props.begin(), props.end());

	sortObjects(objects);

	// look for a visible pixel in the objects' sprite
	for (int i = static_cast<int>(objects.size()) - 1; i >= 0; --i)
	{
		const MapObject* object = objects[i];
		const flat::render::BaseSprite& sprite = object->getSprite();

		flat::video::Color color;
		sprite.getPixel(position, color);
		if (color.a > 0.5f)
		{
			return object;
		}
	}

	return nullptr;
}

void DisplayManager::getEntitiesInAABB(const flat::AABB2& aabb, std::vector<const MapObject*>& entities) const
{
	m_entityQuadtree->getObjects(aabb, entities);
}

TileIndex DisplayManager::getTileIndexAtPosition(const map::fog::Fog& fog, const flat::Vector2& position) const
{
	std::vector<const Tile*> tiles;
	{
		std::vector<TileIndex> tileIndices;
		tileIndices.reserve(8);
		m_tileQuadtree->getObjects(position, tileIndices);

		fog.getTilesFromIndices(tileIndices, tiles);
		sortTiles(tiles);
	}

	// look for a visible pixel in the objects' sprite
	const Tile* tileAtPosition = nullptr;
	for (int i = static_cast<int>(tiles.size()) - 1; i >= 0; --i)
	{
		const Tile* tile = tiles[i];
		const flat::render::BaseSprite& sprite = tile->getSprite();

		flat::video::Color color;
		sprite.getPixel(position, color);
		if (color.a > 0.5f)
		{
			return fog.getTileIndex(tile);
		}
	}

	return TileIndex::INVALID_TILE;
}

#ifdef FLAT_DEBUG
const flat::AABB2& DisplayManager::getEntityCellAABB(const entity::Entity* entity) const
{
	const int cellIndex = m_entityCellIndices.at(entity);
	const EntityQuadTree::Cell& cell = m_entityQuadtree->getCell(cellIndex);
	return cell.getAABB();
}


const flat::AABB2& DisplayManager::getTileCellAABB(const map::TileIndex tileIndex) const
{
	const int cellIndex = m_tileCellIndices.at(tileIndex);
	const TileQuadTree::Cell& cell = m_tileQuadtree->getCell(cellIndex);
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

void DisplayManager::sortTiles(std::vector<const Tile*>& tiles)
{
	std::sort(std::execution::par, tiles.begin(), tiles.end(), locSortByDepth);
}

#ifdef FLAT_DEBUG
void DisplayManager::drawBatches(Game& game, const flat::video::View& view, const std::vector<const MapObject*>& objects, size_t& numObjects, size_t& numDrawCalls)
#else
void DisplayManager::drawBatches(Game& game, const flat::video::View& view, const std::vector<const MapObject*>& objects)
#endif
{
	FLAT_PROFILE("Display Manager Draw Batches");

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



