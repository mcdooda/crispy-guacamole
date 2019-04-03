#ifndef GAME_MAP_DISPLAYMANAGER
#define GAME_MAP_DISPLAYMANAGER

#include <vector>
#include <flat.h>

namespace game
{
class Game;
namespace entity
{
class Entity;
}
namespace map
{
class Map;
class MapObject;
class Tile;

class DisplayManager final
{
	using EntityQuadTree = flat::geometry::QuadTree<const MapObject*, 11, flat::geometry::getDefaultAABB<const MapObject*>>;
	using TerrainQuadTree = flat::geometry::QuadTree<const MapObject*, 11, flat::geometry::getDefaultAABB<const MapObject*>>;
	public:
		DisplayManager();
		
		void addEntity(const entity::Entity* mapObject);
		void removeEntity(const entity::Entity* mapObject);
		void updateEntity(const entity::Entity* mapObject);

		void addTerrainObject(const MapObject* mapObject);
		void removeTerrainObject(const MapObject* mapObject);
		void updateTerrainObject(const MapObject* mapObject);

		void sortAndDraw(Game& game, const flat::video::View& view);

		const MapObject* getObjectAtPosition(const flat::Vector2& position) const;
		void getEntitiesInAABB(const flat::AABB2& aabb, std::vector<const MapObject*>& entities) const;

		const Tile* getTileAtPosition(const flat::Vector2& position) const;

#ifdef FLAT_DEBUG
		const flat::AABB2& getEntityCellAABB(const entity::Entity* entity) const;

		inline size_t getNumOpaqueObjects() const { return m_numOpaqueObjects; }
		inline size_t getNumOpaqueDrawCalls() const { return m_numOpaqueDrawCalls; }

		inline size_t getNumTransparentObjects() const { return m_numTransparentObjects; }
		inline size_t getNumTransparentDrawCalls() const { return m_numTransparentDrawCalls; }
#endif

	private:
		static void sortObjects(std::vector<const MapObject*>& objects);

#ifdef FLAT_DEBUG
		void drawBatches(Game& game, const flat::video::View& view, const std::vector<const MapObject*>& objects, size_t& numObjects, size_t& numDrawCalls);
#else
		void drawBatches(Game& game, const flat::video::View& view, const std::vector<const MapObject*>& objects);
#endif
		
	private:
		std::unique_ptr<flat::render::SpriteBatch> m_spriteBatch;

		std::unique_ptr<EntityQuadTree> m_entityQuadtree;
		std::unordered_map<const MapObject*, int> m_entityCellIndices;

		std::unique_ptr<TerrainQuadTree> m_terrainQuadtree;
		std::unordered_map<const MapObject*, int> m_TerrainObjectCellIndices;

#ifdef FLAT_DEBUG
		size_t m_numOpaqueObjects;
		size_t m_numOpaqueDrawCalls;

		size_t m_numTransparentObjects;
		size_t m_numTransparentDrawCalls;
#endif
};

} // map
} // game

#endif // GAME_MAP_DISPLAYMANAGER



