#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <deque>
#include <functional>

#include <flat.h>

#include "../debug/debugdisplay.h"

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
class Zone;
class DisplayManager;
class TileTemplate;
namespace io
{
class Reader;
}

class Map
{
	public:
		Map();
		virtual ~Map();

		void update(float currentTime);

		void setDisplayManager(DisplayManager* displayManager) { m_displayManager = displayManager; }
		inline DisplayManager& getDisplayManager() const { FLAT_ASSERT(m_displayManager != nullptr); return *m_displayManager; }

		void operator=(Map&&) = delete;
		void operator=(const Map&) = delete;
		
		bool load(Game& game, const mod::Mod& mod);
		bool save(const mod::Mod& mod, const std::string& mapName, const std::vector<entity::Entity*>& entities) const;

		void setBounds(int minX, int maxX, int minY, int maxY);
		void getBounds(int& minX, int& maxX, int& minY, int& maxY) const;
		void getActualBounds(int& minX, int& maxX, int& minY, int& maxY) const;
		
		// get tiles
		const Tile* getTile(int x, int y) const;
		virtual Tile* getTile(int x, int y) = 0;
		const Tile* getTile(float x, float y) const;
		Tile* getTile(float x, float y);

		const Tile* getTileIfExists(int x, int y) const;
		Tile* getTileIfExists(int x, int y);
		const Tile* getTileIfExists(float x, float y) const;
		Tile* getTileIfExists(float x, float y);

		const Tile* getTileIfWalkable(int x, int y) const;
		Tile* getTileIfWalkable(int x, int y);
		const Tile* getTileIfWalkable(float x, float y) const;
		Tile* getTileIfWalkable(float x, float y);

		void eachTile(std::function<void(const Tile*)> func) const;
		virtual void eachTile(std::function<void(Tile*)> func) = 0;
		void eachTileIfExists(std::function<void(const Tile*)> func) const;
		void eachTileIfExists(std::function<void(Tile*)> func);

		flat::render::SpriteSynchronizer& getTileSpriteSynchronizer(const std::shared_ptr<const TileTemplate>& tileTemplate, int tileVariantIndex);
		const std::shared_ptr<const TileTemplate> getTileTemplate(const Tile* tile) const;
		
		// axes
		inline const flat::Matrix3& getTransform() const { return m_transform; }
		inline const flat::Matrix3& getInvTransform() const { return m_invTransform; }
		inline const flat::Vector2& getXAxis() const { return m_xAxis; }
		inline const flat::Vector2& getYAxis() const { return m_yAxis; }
		inline const flat::Vector2& getZAxis() const { return m_zAxis; }
		
		// entities
		void eachEntityInRange(const flat::Vector2& center2d, float range, std::function<void(entity::Entity*)> func) const;

		void setTileNormalDirty(Tile& tile);
		void updateTilesNormals();
		void updateAllTilesNormals();

		std::shared_ptr<Zone>& addZone(const std::string& zoneName);
		bool removeZone(const std::string& zoneName);
		bool getZone(const std::string& zoneName, std::shared_ptr<Zone>& zone) const;
		inline const std::map<std::string, std::shared_ptr<Zone>>& getZones() const { return m_zones; }
		
	protected:
		void setAxes(const flat::Vector2& xAxis,
		             const flat::Vector2& yAxis,
		             const flat::Vector2& zAxis);
		
		virtual void createTiles() = 0;
		
	protected:
		DisplayManager* m_displayManager;

		flat::Matrix3 m_transform;
		flat::Matrix3 m_invTransform;
		flat::Vector2 m_xAxis;
		flat::Vector2 m_yAxis;
		flat::Vector2 m_zAxis;

		int m_minX;
		int m_maxX;
		int m_minY;
		int m_maxY;

		std::vector<Tile*> m_dirtyNormalTiles;

		std::map<std::string, std::shared_ptr<Zone>> m_zones;

		struct TileSpriteSynchronizer
		{
			std::shared_ptr<const map::TileTemplate> tileTemplate;
			int tileVariantIndex;
			flat::render::SpriteSynchronizer spriteSynchronizer;
		};
		std::deque<TileSpriteSynchronizer> m_tileSpriteSynchronizers;
		
	private:
		friend class io::Reader;
};

} // map
} // game

#endif // GAME_MAP_H


