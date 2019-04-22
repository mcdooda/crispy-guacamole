#ifndef GAME_MAP_DISPLAYMANAGER
#define GAME_MAP_DISPLAYMANAGER

#include <vector>
#include <flat.h>

#include "tile.h"

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
class Prop;

class DisplayManager final
{
	using EntityQuadTree = flat::geometry::QuadTree<const entity::Entity*, 2, flat::geometry::getDefaultAABB<const entity::Entity*>>;
	using TileQuadTree = flat::geometry::QuadTree<TileIndex, 7>;
	using PropQuadTree = flat::geometry::QuadTree<PropIndex, 7>;

	public:
		DisplayManager();
		
		void addEntity(const entity::Entity* entity);
		void removeEntity(const entity::Entity* entity);
		void updateEntity(const entity::Entity* entity);

		void addTile(TileIndex tileIndex, const Tile* tile);
		void removeTile(TileIndex tileIndex);
		void updateTile(TileIndex tileIndex, const Tile* tile);

		void addProp(PropIndex propIndex, const Prop* prop);
		void removeProp(PropIndex propIndex);
		void updateProp(PropIndex propIndex, const Prop* prop);

		void sortAndDraw(Game& game, const Map& map, const flat::video::View& view);

		const MapObject* getObjectAtPosition(const Map& map, const flat::Vector2& position) const;
		void getEntitiesInAABB(const flat::AABB2& aabb, std::vector<const MapObject*>& entities) const;

		TileIndex getTileIndexAtPosition(const Map& map, const flat::Vector2& position) const;

#ifdef FLAT_DEBUG
		const flat::AABB2& getEntityCellAABB(const entity::Entity* entity) const;

		inline size_t getNumOpaqueObjects() const { return m_numOpaqueObjects; }
		inline size_t getNumOpaqueDrawCalls() const { return m_numOpaqueDrawCalls; }

		inline size_t getNumTransparentObjects() const { return m_numTransparentObjects; }
		inline size_t getNumTransparentDrawCalls() const { return m_numTransparentDrawCalls; }
#endif

	private:
		static void sortObjects(std::vector<const MapObject*>& objects);
		static void sortTiles(std::vector<const Tile*>& tiles);

#ifdef FLAT_DEBUG
		void drawBatches(Game& game, const flat::video::View& view, const std::vector<const MapObject*>& objects, size_t& numObjects, size_t& numDrawCalls);
#else
		void drawBatches(Game& game, const flat::video::View& view, const std::vector<const MapObject*>& objects);
#endif
		
	private:
		std::unique_ptr<flat::render::SpriteBatch> m_spriteBatch;

		std::unique_ptr<EntityQuadTree> m_entityQuadtree;
		std::unordered_map<const entity::Entity*, int> m_entityCellIndices;

		std::unique_ptr<TileQuadTree> m_tileQuadtree;
		std::unordered_map<TileIndex, int> m_tileCellIndices;

		std::unique_ptr<PropQuadTree> m_propQuadtree;
		std::unordered_map<PropIndex, int> m_propCellIndices;

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



