#ifndef GAME_MAP_DISPLAYMANAGER
#define GAME_MAP_DISPLAYMANAGER

#include <vector>
#include <flat.h>

namespace game
{
namespace map
{
class Map;
class MapObject;

class DisplayManager final
{
	using EntityQuadTree = flat::geometry::QuadTree<MapObject, 3>;
	using TerrainQuadTree = flat::geometry::QuadTree<MapObject, 6>;
	public:
		DisplayManager();
		
		void updateEntities();
		void addEntity(const MapObject* mapObject);
		void removeEntity(const MapObject* mapObject);
		void updateEntity(const MapObject* mapObject);
		void addTerrainObject(const MapObject* mapObject);
		void removeTerrainObject(const MapObject* mapObject);
		void sortByDepthAndDraw(const flat::render::RenderSettings& renderSettings, const flat::video::View& view);
		
	private:
		std::unique_ptr<flat::render::SpriteBatch> m_spriteBatch;
		std::unique_ptr<EntityQuadTree> m_entityQuadtree;
		std::unordered_map<const MapObject*, int> m_entityCellIndices;
		std::unique_ptr<TerrainQuadTree> m_terrainQuadtree;
};

} // map
} // game

#endif // GAME_MAP_DISPLAYMANAGER



