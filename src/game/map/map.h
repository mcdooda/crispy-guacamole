#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <deque>
#include <functional>

#include <flat.h>

#include "tile.h"
#include "../entity/entity.h"
#include "../debug/debugdisplay.h"
#include "../entity/entityhelper.h"

namespace game
{
class Game;
namespace mod
{
class Mod;
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

inline void getEntityAABB(entity::Entity* entity, flat::AABB2& aabb)
{
	const flat::AABB3& aabb3 = entity->getWorldSpaceAABB();
	aabb.min.x = aabb3.min.x;
	aabb.min.y = aabb3.min.y;
	aabb.max.x = aabb3.max.x;
	aabb.max.y = aabb3.max.y;
}

class Map
{
	using EntityQuadTree = flat::geometry::QuadTree<entity::Entity*, 10, getEntityAABB>;
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

		const Tile* getTileIfNavigable(int x, int y, Navigability navigabilityMask) const;
		Tile* getTileIfNavigable(int x, int y, Navigability navigabilityMask);
		const Tile* getTileIfNavigable(float x, float y, Navigability navigabilityMask) const;
		Tile* getTileIfNavigable(float x, float y, Navigability navigabilityMask);

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
		int addEntity(entity::Entity* entity);
		void removeEntity(entity::Entity* entity, int cellIndex);
		int updateEntityPosition(entity::Entity* entity, int cellIndex);

		template <typename Func>
		void eachTileEntity(const Tile* tile, Func func) const;
		int getTileEntityCount(const Tile* tile) const;

		template <class Func>
		inline void eachEntityInRange(const flat::Vector2& center2d, float range, Func func) const;

		template <class Func>
		inline void eachEntityInCollisionRange(const flat::Vector2& center2d, float range, Func func) const;

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

		std::unique_ptr<EntityQuadTree> m_entityQuadtree;
		
	private:
		friend class io::Reader;
};


template <typename Func>
void Map::eachTileEntity(const Tile* tile, Func func) const
{
	const flat::AABB3& aabb3 = tile->getWorldSpaceAABB();
	flat::AABB2 tileAABB(
		flat::Vector2(aabb3.min),
		flat::Vector2(aabb3.max)
	);
	m_entityQuadtree->eachObject(
		tileAABB,
		[&tileAABB, &func](entity::Entity* entity)
		{
			if (tileAABB.isInside(flat::Vector2(entity->getPosition())))
			{
				func(entity);
			}
		}
	);
}

template <class Func>
inline void Map::eachEntityInRange(const flat::Vector2& center2d, float range, Func func) const
{
	flat::AABB2 rangeAABB(
		center2d - flat::Vector2(range, range),
		center2d + flat::Vector2(range, range)
	);
	const float range2 = range * range;
	flat::containers::HybridArray<entity::Entity*, 128> entitiesInRange;
	m_entityQuadtree->eachObject(
		rangeAABB,
		[&func, &center2d, range2, &entitiesInRange](entity::Entity* entity)
		{
			flat::Vector2 entityPosition2d(entity->getPosition());
			if (flat::length2(center2d - entityPosition2d) <= range2)
			{
				func(entity);
			}
		}
	);
}


template <class Func>
void Map::eachEntityInCollisionRange(const flat::Vector2& center2d, float radius, Func func) const
{
	flat::AABB2 radiusAABB(
		center2d - flat::Vector2(radius, radius),
		center2d + flat::Vector2(radius, radius)
	);
	m_entityQuadtree->eachObject(
		radiusAABB,
		[&func, &center2d, radius](entity::Entity* entity)
		{
			flat::Vector2 entityPosition2d(entity->getPosition());
			const float entityRadius = entity::EntityHelper::getRadius(entity);
			if (flat::length2(center2d - entityPosition2d) < flat::square(radius + entityRadius))
			{
				func(entity);
			}
		}
	);
}

} // map
} // game

#endif // GAME_MAP_H


