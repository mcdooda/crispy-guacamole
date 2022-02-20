#include <execution>

#include "map/displaymanager.h"
#include "map/mapobject.h"
#include "map/tile.h"
#include "map/prop.h"
#include "map/fog/fog.h"

#include "entity/entity.h"

#include "game.h"
#include "states/basemapstate.h"

#define DEBUG_DRAW 0
#define DRAW_3D_TILES 1

namespace game::map
{

inline bool locSortByDepthXY(const MapObject* a, const MapObject* b);

DisplayManager::DisplayManager()
#ifdef FLAT_DEBUG
	: m_numOpaqueObjects(0)
	, m_numOpaqueDrawCalls(0)
	, m_numTransparentObjects(0)
	, m_numTransparentDrawCalls(0)
#endif
{
	m_spriteBatch = std::make_unique<flat::render::SpriteBatch>();
	m_meshBatch = std::make_unique<flat::render::MeshBatch>();
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
	if (entity->getSprite() != nullptr)
	{
		entity->updateRenderHash();
		int cellIndex = m_entityQuadtree->addObject(entity);
		m_entityCellIndices[entity] = cellIndex;
	}
}

void DisplayManager::removeEntity(const entity::Entity* entity)
{
	if (entity->getSprite() != nullptr)
	{
		int cellIndex = m_entityCellIndices.at(entity);
		m_entityQuadtree->removeObject(entity, cellIndex);
		m_entityCellIndices.erase(entity);
	}
}

void DisplayManager::updateEntity(const entity::Entity* entity)
{
	if (entity->getSprite() != nullptr)
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

void DisplayManager::draw(Game& game, const map::fog::Fog& fog, const flat::video::View& view)
{
#if DRAW_3D_TILES
	drawMeshes(game, fog, view);
#endif
	//drawSprites(game, fog, view);
}

void DisplayManager::drawMeshes(Game& game, const map::fog::Fog& fog, const flat::video::View& view)
{
	flat::AABB2 screenAABB;
	view.getScreenAABB(screenAABB);
	screenAABB.min *= 3.f;
	screenAABB.max *= 3.f;

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

	std::vector<const MapObject*> objects;
	objects.reserve(tiles.size() + props.size());

	for (const Tile* tile : tiles)
	{
		objects.push_back(tile);
	}

	for (const Prop* prop : props)
	{
		objects.push_back(prop);
	}

	std::sort(
		std::execution::par,
		objects.begin(),
		objects.end(),
		[](const MapObject* a, const MapObject* b)
		{
			return a->getRenderHash() < b->getRenderHash();
		}
	);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	drawMeshBatches(game, view, objects, m_numOpaqueObjects, m_numOpaqueDrawCalls);

	glDisable(GL_DEPTH_TEST);
}

void DisplayManager::drawSprites(Game& game, const map::fog::Fog& fog, const flat::video::View& view)
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

#if !DRAW_3D_TILES
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
#endif // !DRAW_3D_TILES

	// put everything into a single vector
	std::vector<const MapObject*> objects;

	size_t numObjects = m_temporaryObjects.size() + numEntities;
#if !DRAW_3D_TILES
	numObjects += tiles.size() + props.size();
#endif // !DRAW_3D_TILES
	objects.reserve(numObjects);

#if !DRAW_3D_TILES
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
#endif // !DRAW_3D_TILES


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
		//sortObjects(objects, fog, screenAABB, true);

		std::sort(
			std::execution::par,
			objects.begin(),
			objects.end(),
			[](const MapObject* a, const MapObject* b)
			{
				return a->getRenderHash() < b->getRenderHash();
			}
		);
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
			const_cast<flat::render::BaseSprite*>(mapObject->getSprite())->setDepth(depth);
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
			[](const MapObject* a) { return a->getSprite()->requiresAlphaBlending(); }
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
		drawSpriteBatches(game, view, opaqueObjects, m_numOpaqueObjects, m_numOpaqueDrawCalls);
	}

	{
		FLAT_PROFILE("Display Manager Draw Transparent Objects");

		std::vector<const MapObject*> transparentObjects = objects;
		transparentObjects.erase(std::remove_if(
			transparentObjects.begin(),
			transparentObjects.end(),
			[](const MapObject* a) { return !a->getSprite()->requiresAlphaBlending(); }
		), transparentObjects.end());

		glEnable(GL_BLEND);
		drawSpriteBatches(game, view, transparentObjects, m_numTransparentObjects, m_numTransparentDrawCalls);
	}

	glDisable(GL_DEPTH_TEST);
}

const MapObject* DisplayManager::getObjectAtPosition(const map::fog::Fog& fog, const flat::Vector2& position, const flat::video::View& view) const
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

	flat::AABB2 screenAABB;
	view.getScreenAABB(screenAABB);
	sortObjects(objects, fog, screenAABB, false);

	// look for a visible pixel in the objects' sprite
	for (int i = static_cast<int>(objects.size()) - 1; i >= 0; --i)
	{
		const MapObject* object = objects[i];
		const flat::render::BaseSprite* sprite = object->getSprite();

		flat::video::Color color;
		sprite->getPixel(position, color);
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
		const flat::render::BaseSprite* sprite = tile->getSprite();

		flat::video::Color color;
		sprite->getPixel(position, color);
		if (color.a > 0.5f)
		{
			return fog.getTileIndex(tile);
		}
	}

	return TileIndex::INVALID_VALUE;
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

inline bool locSortByDepthXY(const MapObject* a, const MapObject* b)
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

inline bool locSortByAABB(const MapObject* a, const MapObject* b)
{
	const flat::AABB3& aAABB = a->getWorldSpaceAABB();
	const flat::AABB3& bAABB = b->getWorldSpaceAABB();
	if (flat::AABB3::overlap(aAABB, bAABB))
	{
		return locSortByDepthXY(a, b);
	}
	else if (aAABB.getSize().z == 0.f && bAABB.getSize().z == 0.f && flat::AABB3::overlapInclusive(aAABB, bAABB))
	{
		return locSortByDepthXY(a, b);
	}
	else if (aAABB.getSize().z == 0.f || bAABB.getSize().z == 0.f)
	{
		const float aZ = aAABB.getCenter().z;
		const float bZ = bAABB.getCenter().z;
		if (aZ == bZ)
		{
			return locSortByDepthXY(a, b);
		}
		else
		{
			return aZ < bZ;
		}
	}
	else
	{
		const bool aBeforeB = aAABB.max.x < bAABB.min.x || aAABB.max.y < bAABB.min.y || aAABB.max.z < bAABB.min.z;
		const bool bBeforeA = aAABB.max.x < bAABB.min.x || aAABB.max.y < bAABB.min.y || aAABB.max.z < bAABB.min.z;
		if (aBeforeB == bBeforeA)
		{
			return locSortByDepthXY(a, b);
		}
		else
		{
			return aBeforeB;
		}
	}
}

void DisplayManager::sortObjects(std::vector<const MapObject*>& objects, const map::fog::Fog& fog, flat::AABB2 screenAABB, bool renderingWholeScreen) const
{
	// 1st pass: sort by simple depth using x + y
	std::sort(std::execution::par, objects.begin(), objects.end(), locSortByDepthXY);
	FLAT_ASSERT(std::is_sorted(objects.begin(), objects.end(), locSortByDepthXY));

#if DEBUG_DRAW
	for (const MapObject* o : objects)
	{
		const_cast<MapObject*>(o)->getSprite().setColor(flat::video::Color::WHITE);

#ifdef FLAT_DEBUG
		// ensure that the sprite's AABB is update to date
		const flat::AABB2& currentSpriteAABB = o->getAABB();
		flat::AABB2 desiredSpriteAABB;
		o->getSprite().getAABB(desiredSpriteAABB);
		FLAT_ASSERT(desiredSpriteAABB == currentSpriteAABB);
#endif
	}
#endif

#if !DRAW_3D_TILES
	// 2nd pass: move entities in front of tiles if their sprites overlap and the entity is higher in altitude
	int numTileSwaps = 0;
	for (int i = 0, e = static_cast<int>(objects.size()); i < e; ++i)
	{
		const MapObject* mapObject = objects[i];
		if (mapObject->isEntity())
		{
			const flat::AABB2& spriteAABB = mapObject->getAABB();
			const flat::AABB3& AABB = mapObject->getWorldSpaceAABB();
			const flat::Vector3 center = AABB.getCenter();

			std::vector<TileIndex> overlappingTileIndices;
			overlappingTileIndices.reserve(16);
			{
				FLAT_PROFILE("Display Manager Get Tiles");
				m_tileQuadtree->getObjects(spriteAABB, overlappingTileIndices);
			}
			std::vector<const Tile*> overlappingTiles;
			fog.getTilesFromIndices(overlappingTileIndices, overlappingTiles);

			// find tiles to move the entity in front of
			int nearestTileIndex = -1;
			for (int j = 0; j < static_cast<int>(overlappingTiles.size()); ++j)
			{
				const Tile* tile = overlappingTiles[j];

				// ignore the tile if it's not on screen
				if (!flat::AABB2::overlap(tile->getAABB(), screenAABB))
				{
					continue;
				}

				if (!renderingWholeScreen)
				{
					if (std::find(objects.begin(), objects.end(), tile) == objects.end())
					{
						continue;
					}
				}

#if DEBUG_DRAW
				FLAT_ASSERT(std::find(objects.begin(), objects.end(), tile) != objects.end());
#endif

				// ignore the tile if it's already behind the entity
				if (locSortByDepthXY(tile, mapObject))
				{
					continue;
				}

				// if the tile is below the entity in altitude, the entity should be moved forward
				const flat::AABB3& tileAABB = tile->getWorldSpaceAABB();
				if (tileAABB.max.z <= AABB.min.z)
				{
					if (nearestTileIndex == -1)
					{
						nearestTileIndex = j;
					}
					else if (!locSortByDepthXY(tile, overlappingTiles[nearestTileIndex]))
					{
						nearestTileIndex = j;
#if DEBUG_DRAW
						const_cast<Tile*>(tile)->getSprite().setColor(flat::video::Color::GREEN);
#endif
					}
				}
			}

			// move the entity in front of that tile
			if (nearestTileIndex != -1)
			{
				const MapObject* tile = overlappingTiles[nearestTileIndex];
				std::vector<const MapObject*>::iterator it = std::find(objects.begin(), objects.end(), tile);
				FLAT_ASSERT(it != objects.end());
				const int k = static_cast<int>(it - objects.begin());

				if (k > i)
				{
					std::rotate(objects.begin() + i, objects.begin() + i + 1, objects.begin() + k + 1);

#if DEBUG_DRAW
					const_cast<MapObject*>(mapObject)->getSprite().setColor(flat::video::Color::RED);
					const_cast<MapObject*>(tile)->getSprite().setColor(flat::video::Color::BLUE);
#endif

					++numTileSwaps;
					FLAT_ASSERT(numTileSwaps < objects.size());

					--i;
				}
			}
		}
	}
#endif // !DRAW_3D_TILES

	// 3rd pass: resort entities between each other using their AABB
	// see https://shaunlebron.github.io/IsometricBlocks/ for AABB depth sorting
	std::map<const MapObject*, std::set<const MapObject*>> objectsBehind;
	std::map<const MapObject*, std::set<const MapObject*>> objectsInFront;
	int numEntities = 0;
	for (int i = 0, e = static_cast<int>(objects.size()); i < e; ++i)
	{
		const MapObject* mapObject = objects[i];
		if (mapObject->isEntity())
		{
			++numEntities;

			const flat::AABB2& spriteAABB = mapObject->getAABB();
			const flat::AABB3& AABB = mapObject->getWorldSpaceAABB();
			const flat::Vector3 center = AABB.getCenter();

			std::vector<const entity::Entity*> overlappingEntities;
			overlappingEntities.reserve(16);
			m_entityQuadtree->getObjects(spriteAABB, overlappingEntities);

			for (int j = static_cast<int>(overlappingEntities.size() - 1); j >= 0; --j)
			{
				const entity::Entity* overlappingEntity = overlappingEntities[j];
				const TileIndex tileIndex = overlappingEntity->getTileIndexFromPosition();
				if (!fog.isTileDiscovered(tileIndex)
				 || overlappingEntity == mapObject
				 || !flat::AABB2::overlap(overlappingEntity->getAABB(), screenAABB)
				 || (!renderingWholeScreen && std::find(objects.begin(), objects.end(), overlappingEntity) == objects.end()))
				{
					if (!overlappingEntities.empty())
					{
						overlappingEntities[j] = overlappingEntities[overlappingEntities.size() - 1];
						overlappingEntities.pop_back();
					}
					else
					{
						overlappingEntities.clear();
					}
				}
			}

			for (int j = 0; j < static_cast<int>(overlappingEntities.size()); ++j)
			{
				const MapObject* otherMapObject = overlappingEntities[j];

#if DEBUG_DRAW
				FLAT_ASSERT(std::find(objects.begin(), objects.end(), otherMapObject) != objects.end());

				FLAT_ASSERT(locSortByAABB(otherMapObject, mapObject) == !locSortByAABB(mapObject, otherMapObject));
#endif

				if (locSortByAABB(otherMapObject, mapObject))
				{
					objectsBehind[mapObject].insert(otherMapObject);
				}
				else
				{
					objectsInFront[mapObject].insert(otherMapObject);
				}
			}
		}
	}

	std::deque<const MapObject*> entitiesToDraw;
	for (int i = 0, e = static_cast<int>(objects.size()); i < e; ++i)
	{
		const MapObject* mapObject = objects[i];
		if (mapObject->isEntity())
		{
			if (objectsBehind.count(mapObject) == 0)
			{
				entitiesToDraw.push_back(mapObject);
			}
		}
	}
	FLAT_ASSERT(numEntities == 0 || !entitiesToDraw.empty());

	std::vector<const MapObject*> entitiesDrawn;
	std::map<const MapObject*, int> entitiesDrawnIndex;
	while (!entitiesToDraw.empty())
	{
		const MapObject* mapObject = entitiesToDraw.front();
		entitiesToDraw.pop_front();

		const int entityDrawnIndex = static_cast<int>(entitiesDrawn.size());
		entitiesDrawnIndex[mapObject] = entityDrawnIndex;
		entitiesDrawn.push_back(mapObject);

		std::set<const MapObject*>& objectInFrontOfMapObject = objectsInFront[mapObject];
		for (std::set<const MapObject*>::iterator it2 = objectInFrontOfMapObject.begin(); it2 != objectInFrontOfMapObject.end(); ++it2)
		{
			const MapObject* mapObject2 = *it2;
			if (!objectsBehind[mapObject2].empty())
			{
				objectsBehind[mapObject2].erase(mapObject);
				if (objectsBehind[mapObject2].empty())
				{
					entitiesToDraw.push_back(mapObject2);
				}
			}
		}
	}

	FLAT_ASSERT(entitiesDrawn.size() == numEntities);

	int numEntitySwaps = 0;
	for (int i = 0, e = static_cast<int>(entitiesDrawn.size() - 1); i < e; ++i)
	{
		const MapObject* mapObject1 = entitiesDrawn[i];
		std::vector<const MapObject*>::iterator it1 = std::find(objects.begin(), objects.end(), mapObject1);
		FLAT_ASSERT(it1 != objects.end());

		const MapObject* mapObject2 = entitiesDrawn[i + 1];
		std::vector<const MapObject*>::iterator it2 = std::find(objects.begin(), objects.end(), mapObject2);
		FLAT_ASSERT(it2 != objects.end());

		if (it1 > it2)
		{
			std::rotate(it2, it2 + 1, it1 + 1);
			++numEntitySwaps;
		}
	}

#if DEBUG_DRAW
	// ensure that this entities are drawn in the expected order
	int currentEntityIndex = 0;
	for (int i = 0, e = static_cast<int>(objects.size()); i < e; ++i)
	{
		const MapObject* mapObject = objects[i];
		if (mapObject->isEntity())
		{
			std::map<const MapObject*, int>::iterator it = entitiesDrawnIndex.find(mapObject);
			FLAT_ASSERT(it != entitiesDrawnIndex.end());
			if (it->second != currentEntityIndex)
			{
				for (std::map<const MapObject*, int>::iterator it = entitiesDrawnIndex.begin(); it != entitiesDrawnIndex.end(); ++it)
				{
					std::cout << it->first << " -> " << it->second << std::endl;
				}
				std::cout << std::endl;
				FLAT_BREAK();
			}
			++currentEntityIndex;
		}
	}

	if (renderingWholeScreen)
	{
		std::cout << "tile swaps: " << numTileSwaps << std::endl
		          << "entity swaps: " << numEntitySwaps << std::endl << std::endl;

		for (int i = 0; i < static_cast<int>(entitiesDrawn.size()); ++i)
		{
			std::cout << i << " " << static_cast<const entity::Entity*>(entitiesDrawn[i])->getTemplateName() << std::endl;
		}
		std::cout << std::endl;
	}
#endif
}

void DisplayManager::sortTiles(std::vector<const Tile*>& tiles)
{
	std::sort(std::execution::par, tiles.begin(), tiles.end(), locSortByDepthXY);
}

void DisplayManager::drawSpriteBatches(Game& game, const flat::video::View& view, const std::vector<const MapObject*>& objects, size_t& numObjects, size_t& numDrawCalls)
{
	FLAT_PROFILE("Display Manager Draw Sprite Batches");

#ifdef FLAT_DEBUG
	numObjects = objects.size();
	numDrawCalls = 0;
#endif

	flat::video::Window& window = *game.video->window;

	flat::render::SpriteBatch* spriteBatch = m_spriteBatch.get();

	flat::state::State* state = game.getStateMachine().getState();
	const states::BaseMapState& mapState = state->as<states::BaseMapState>();
	const Map& map = mapState.getMap();

	const flat::Matrix4& viewMatrix = view.getViewProjectionMatrix();

	std::vector<const MapObject*>::const_iterator it = objects.begin();
	std::vector<const MapObject*>::const_iterator end = objects.end();
	while (it != end)
	{
		spriteBatch->clear();

		std::vector<const MapObject*>::const_iterator it2 = it;
		while (it2 != end && (*it2)->getRenderHash() == (*it)->getRenderHash())
		{
			FLAT_ASSERT((*it2)->getSprite() != nullptr);
			spriteBatch->add(*(*it2)->getSprite());
			++it2;
		}

		const flat::render::ProgramSettings& programSettings = (*it)->getProgramSettings();
		it = it2;

		programSettings.program.use(window);
		programSettings.settings.viewProjectionMatrixUniform.set(viewMatrix);
		spriteBatch->draw(programSettings.settings, viewMatrix);

#ifdef FLAT_DEBUG
		++numDrawCalls;
#endif
	}
}

void DisplayManager::drawMeshBatches(Game& game, const flat::video::View& view, const std::vector<const MapObject*>& objects, size_t& numObjects, size_t& numDrawCalls)
{
	FLAT_PROFILE("Display Manager Draw Mesh Batches");

#ifdef FLAT_DEBUG
	numObjects = objects.size();
	numDrawCalls = 0;
#endif

	flat::video::Window& window = *game.video->window;

	flat::render::MeshBatch* meshBatch = m_meshBatch.get();

	flat::state::State* state = game.getStateMachine().getState();
	const states::BaseMapState& mapState = state->as<states::BaseMapState>();
	const Map& map = mapState.getMap();

	const flat::Matrix4 viewProjectionMatrix = view.getViewProjectionMatrix();
	const float angleAroundZ = std::sin(game.time->defaultClock->getTime() * 0.5f) * flat::PI * 0.125f;
	flat::Matrix4 mapAxesMatrix = flat::Matrix4(map.getTransform());
	flat::rotateZBy(mapAxesMatrix, angleAroundZ);
	const flat::Matrix4 finalViewProjectionMatrix = viewProjectionMatrix * mapAxesMatrix;

	std::vector<const MapObject*>::const_iterator it = objects.begin();
	std::vector<const MapObject*>::const_iterator end = objects.end();

	/*
	float minZ = FLT_MAX;
	float maxZ = -FLT_MAX;
	*/
	while (it != end)
	{
		meshBatch->clear();

		std::vector<const MapObject*>::const_iterator it2 = it;
		while (it2 != end && (*it2)->getRenderHash() == (*it)->getRenderHash())
		{
			FLAT_ASSERT((*it2)->getMesh() != nullptr);
			const flat::render::Mesh& mesh = *(*it2)->getMesh();
			meshBatch->add(mesh);
			++it2;
		}

		const flat::render::ProgramSettings& programSettings = (*it)->getProgramSettings();
		it = it2;

		programSettings.program.use(window);
		programSettings.settings.viewProjectionMatrixUniform.set(finalViewProjectionMatrix);
		meshBatch->draw(programSettings.settings, finalViewProjectionMatrix);

#ifdef FLAT_DEBUG
		++numDrawCalls;
#endif
	}

	//std::cout << "minZ = " << minZ << ", maxZ = " << maxZ << std::endl;
}

#undef DEBUG_DRAW

} // game::map
