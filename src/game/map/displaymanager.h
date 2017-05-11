#ifndef GAME_MAP_DISPLAYMANAGER
#define GAME_MAP_DISPLAYMANAGER

#include <vector>
#include <flat.h>

namespace game
{
namespace entity
{
class Entity;
}
namespace map
{
class Map;
class MapObject;

class DisplayManager final
{
	using EntityQuadTree = flat::geometry::QuadTree<MapObject, 8>;
	using TerrainQuadTree = flat::geometry::QuadTree<MapObject, 8>;
	public:
		DisplayManager();
		
		void addEntity(const entity::Entity* mapObject);
		void removeEntity(const entity::Entity* mapObject);
		void updateEntity(const entity::Entity* mapObject);

		void addTerrainObject(const MapObject* mapObject);
		void removeTerrainObject(const MapObject* mapObject);
		void updateTerrainObject(const MapObject* mapObject);

		void sortByDepthAndDraw(const flat::render::RenderSettings& renderSettings, const flat::video::View& view);

		const MapObject* getObjectAtPosition(const flat::Vector2& position) const;

	private:
		static void sortObjectsByDepth(std::vector<const MapObject*>& objects);
		
	private:
		std::unique_ptr<flat::render::SpriteBatch> m_spriteBatch;

		std::unique_ptr<EntityQuadTree> m_entityQuadtree;
		std::unordered_map<const MapObject*, int> m_entityCellIndices;

		std::unique_ptr<TerrainQuadTree> m_terrainQuadtree;
		std::unordered_map<const MapObject*, int> m_TerrainObjectCellIndices;
};

} // map
} // game

#endif // GAME_MAP_DISPLAYMANAGER



