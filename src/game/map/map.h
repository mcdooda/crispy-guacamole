#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <functional>
#include <flat.h>

namespace game
{
class Game;
namespace mod
{
class Mod;
}
namespace entity
{
class Entity;
}
namespace map
{
class Tile;
class DisplayManager;
namespace io
{
class Reader;
}

class Map final
{
	public:
		Map();
		~Map();
		
		void load(Game* game, const mod::Mod& mod);
		
		void drawTiles(DisplayManager& displayManager, const flat::video::View& view) const;
		
		const Tile* getTile(int x, int y) const;
		Tile* getTile(int x, int y);
		const Tile* getTileIfExists(int x, int y) const;
		Tile* getTileIfExists(int x, int y);
		const Tile* getTileIfWalkable(int x, int y) const;
		Tile* getTileIfWalkable(int x, int y);
		
		void eachTile(std::function<void(const Tile*)> func) const;
		void eachTile(std::function<void(Tile*)> func);
		
		inline const flat::geometry::Vector2& getXAxis() const { return m_xAxis; }
		inline const flat::geometry::Vector2& getYAxis() const { return m_yAxis; }
		inline const flat::geometry::Vector2& getZAxis() const { return m_zAxis; }
		
		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }
		
		inline int getTileWidth() const { return m_tileWidth; }
		inline int getTileHeight() const { return m_tileHeight; }
		
		void addEntity(entity::Entity* entity);
		void removeEntity(entity::Entity* entity);
		
		void updateEntities(float currentTime, float elapsedTime);
		
	private:
		int getTileIndex(int x, int y) const;
		int getNumTiles() const;
		
		void setSize(int width, int height);
		void setTileSize(int tileWidth, int tileHeight);
		
		void createTiles();
		void destroyTiles();
		
		void destroyEntities();
		
	private:
		int m_width;
		int m_height;
		
		int m_tileWidth;
		int m_tileHeight;
		
		Tile* m_tiles;
		
		flat::geometry::Vector2 m_xAxis;
		flat::geometry::Vector2 m_yAxis;
		flat::geometry::Vector2 m_zAxis;
		
		std::vector<entity::Entity*> m_entities;
		
	private:
		friend class io::Reader;
};

} // map
} // game

#endif // GAME_MAP_H


