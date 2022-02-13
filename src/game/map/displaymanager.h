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
class MapObject;
class Tile;
class Prop;

namespace fog
{
class Fog;
}

class DisplayManager final
{
	using EntityQuadTree = flat::geometry::QuadTree<const entity::Entity*, 2, flat::geometry::getDefaultAABB<const entity::Entity*>>;
	using TileQuadTree = flat::geometry::QuadTree<TileIndex, 7>;
	using PropQuadTree = flat::geometry::QuadTree<PropIndex, 7>;

	public:
		DisplayManager();

		void clear();

		void addEntity(const entity::Entity* entity);
		void removeEntity(const entity::Entity* entity);
		void updateEntity(const entity::Entity* entity);

		void addTile(TileIndex tileIndex, const Tile* tile);
		void removeTile(TileIndex tileIndex);
		void updateTile(TileIndex tileIndex, const Tile* tile);
		void moveTileIndex(TileIndex fromIndex, TileIndex toIndex);

		void addProp(PropIndex propIndex, const Prop* prop);
		void removeProp(PropIndex propIndex);
		void updateProp(PropIndex propIndex, const Prop* prop);
		void movePropIndex(PropIndex fromIndex, PropIndex toIndex);

		void addTemporaryObject(const MapObject* mapObject);

		void draw(Game& game, const map::fog::Fog& fog, const flat::video::View& view);

		const MapObject* getObjectAtPosition(const map::fog::Fog& fog, const flat::Vector2& position, const flat::video::View& view) const;
		void getEntitiesInAABB(const flat::AABB2& aabb, std::vector<const MapObject*>& entities) const;

		TileIndex getTileIndexAtPosition(const map::fog::Fog& fog, const flat::Vector2& position) const;

#ifdef FLAT_DEBUG
		const flat::AABB2& getEntityCellAABB(const entity::Entity* entity) const;

		const flat::AABB2& getTileCellAABB(const map::TileIndex tileIndex) const;

		inline size_t getNumOpaqueObjects() const { return m_numOpaqueObjects; }
		inline size_t getNumOpaqueDrawCalls() const { return m_numOpaqueDrawCalls; }

		inline size_t getNumTransparentObjects() const { return m_numTransparentObjects; }
		inline size_t getNumTransparentDrawCalls() const { return m_numTransparentDrawCalls; }
#endif

	private:
		void sortObjects(std::vector<const MapObject*>& objects, const map::fog::Fog& fog, flat::AABB2 screenAABB, bool renderingWholeScreen) const;
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

		std::vector<const MapObject*> m_temporaryObjects;

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



