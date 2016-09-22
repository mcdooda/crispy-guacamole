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

		void operator=(Map&& other);
		
		bool load(lua_State* L, Game* game, const mod::Mod& mod, const std::string& mapName);

		bool save(Game* game, const mod::Mod& mod, const std::string& mapName) const;
		
		void drawTiles(DisplayManager& displayManager, const flat::video::View& view) const;
		
		const Tile* getTile(int x, int y) const;
		Tile* getTile(int x, int y);
		const Tile* getTile(float x, float y) const { return getTile(static_cast<int>(std::floor(x + 0.5f)), static_cast<int>(std::floor(y + 0.5f))); }
		Tile* getTile(float x, float y) { return getTile(static_cast<int>(std::floor(x + 0.5f)), static_cast<int>(std::floor(y + 0.5f))); }

		const Tile* getTileIfExists(int x, int y) const;
		Tile* getTileIfExists(int x, int y);
		const Tile* getTileIfExists(float x, float y) const { return getTileIfExists(static_cast<int>(std::floor(x + 0.5f)), static_cast<int>(std::floor(y + 0.5f))); }
		Tile* getTileIfExists(float x, float y) { return getTileIfExists(static_cast<int>(std::floor(x + 0.5f)), static_cast<int>(std::floor(y + 0.5f))); }

		const Tile* getTileIfWalkable(int x, int y) const;
		Tile* getTileIfWalkable(int x, int y);
		const Tile* getTileIfWalkable(float x, float y) const { return getTileIfWalkable(static_cast<int>(std::floor(x + 0.5f)), static_cast<int>(std::floor(y + 0.5f))); }
		Tile* getTileIfWalkable(float x, float y) { return getTileIfWalkable(static_cast<int>(std::floor(x + 0.5f)), static_cast<int>(std::floor(y + 0.5f))); }

		
		void eachTile(std::function<void(const Tile*)> func) const;
		void eachTile(std::function<void(Tile*)> func);
		void eachTileTopToDown(std::function<void(const Tile*)> func) const;
		
		inline const flat::Vector2& getXAxis() const { return m_xAxis; }
		inline const flat::Vector2& getYAxis() const { return m_yAxis; }
		inline const flat::Vector2& getZAxis() const { return m_zAxis; }
		
		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }
		
		void addEntity(entity::Entity* entity);
		void removeEntity(entity::Entity* entity);

		inline void removeAllEntities() { m_entities.clear(); }

		const std::vector<entity::Entity*>& getEntities() const { return m_entities; }
		std::vector<entity::Entity*>& getEntities() { return m_entities; }
		
		void updateEntities(float currentTime, float elapsedTime);
		
	private:
		int getTileIndex(int x, int y) const;
		int getNumTiles() const;
		
		void setSize(int width, int height);
		void setAxes(const flat::Vector2& xAxis,
		             const flat::Vector2& yAxis,
		             const flat::Vector2& zAxis);
		
		void createTiles();
		void destroyTiles();
		
	private:
		int m_width;
		int m_height;
		
		Tile* m_tiles;
		
		flat::Vector2 m_xAxis;
		flat::Vector2 m_yAxis;
		flat::Vector2 m_zAxis;
		
		std::vector<entity::Entity*> m_entities;
		
	private:
		friend class io::Reader;
};

} // map
} // game

#endif // GAME_MAP_H


