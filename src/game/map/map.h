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
	private:
		using EntityQuadTree = flat::geometry::QuadTree<entity::Entity*, 10, getEntityAABB>;

		struct TileNavigation
		{
			float z;
			Navigability navigability;
		};

		struct TilePosition
		{
			flat::Vector2i xy;
		};

		struct NeighborTiles
		{
			static constexpr int MAX_NEIGHBORS = 4;
			std::array<TileIndex, MAX_NEIGHBORS> neighbors;
			NeighborTiles() :
				neighbors{
					TileIndex::INVALID_TILE,
					TileIndex::INVALID_TILE,
					TileIndex::INVALID_TILE,
					TileIndex::INVALID_TILE
				}
			{}
		};

	public:
		Map();
		~Map();

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
		TileIndex createTile(const flat::Vector2i& xy, float z, uint16_t tileTemplateVariantIndex, std::shared_ptr<const TileTemplate> tileTemplate);
		void deleteTile(TileIndex tileIndex);
		void deleteTile(const flat::Vector2i& tilePosition);

		TileIndex getTileIndex(int x, int y) const;
		TileIndex getTileIndex(const flat::Vector2i& position) const;
		TileIndex getTileIndex(float x, float y) const;
		TileIndex getTileIndex(const Tile* tile) const;

		void getTilesFromIndices(const std::vector<TileIndex>& tileIndices, std::vector<const Tile*>& tiles) const;
		void getPropsFromIndices(const std::vector<PropIndex>& propIndices, std::vector<const Prop*>& props) const;

		const flat::Vector2i& getTileXY(TileIndex tileIndex) const;
		void setTileZ(TileIndex tileIndex, float z);
		void moveTileZBy(TileIndex tileIndex, float dz);
		float getTileZ(TileIndex tileIndex) const;

		TileIndex getTileIndexIfNavigable(int x, int y, Navigability navigabilityMask) const;
		TileIndex getTileIndexIfNavigable(float x, float y, Navigability navigabilityMask) const;

		bool isTileNavigable(TileIndex tileIndex, Navigability navigabilityMask) const;
		Navigability getTileNavigability(TileIndex tileIndex) const;
		void setTileNavigability(TileIndex tileIndex, Navigability navigability);

		void setTileColor(TileIndex tileIndex, const flat::video::Color& color);
		const flat::video::Color& getTileColor(TileIndex tileIndex) const;

		void setTilePropTexture(TileIndex tileIndex, std::shared_ptr<const flat::video::Texture> texture);
		void removeTileProp(TileIndex tileIndex);
		const Prop* getTileProp(TileIndex tileIndex) const;

		const flat::render::BaseSprite& getTileSprite(TileIndex tileIndex) const;
		flat::render::BaseSprite& getTileSprite(TileIndex tileIndex);
		void synchronizeTileSpriteTo(TileIndex tileIndex, flat::render::SpriteSynchronizer& synchronizer);

		flat::render::SpriteSynchronizer& getTileSpriteSynchronizer(const std::shared_ptr<const TileTemplate>& tileTemplate, int tileVariantIndex);
		const std::shared_ptr<const TileTemplate> getTileTemplate(TileIndex tileIndex) const;

#ifdef FLAT_DEBUG
		void checkTilePropIndicesIntegrity() const;
#endif
		
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
		void eachTileEntity(TileIndex tileIndex, Func func) const;
		int getTileEntityCount(TileIndex tileIndex) const;

		template <class Func>
		void eachTile(Func func) const;

		void buildNeighborTiles();

		template <class Func>
		void eachNeighborTiles(TileIndex tileIndex, Func func) const;

		template <class Func>
		void eachNeighborTilesWithNavigability(TileIndex tileIndex, float jumpHeight, Navigability navigabilityMask, Func func) const;

		template <class Func>
		inline void eachEntityInRange(const flat::Vector2& center2d, float range, Func func) const;

		template <class Func>
		inline void eachEntityInCollisionRange(const flat::Vector2& center2d, float range, Func func) const;

		void setNeighborTilesDirty(TileIndex tileIndex);
		void setTileNormalDirty(TileIndex tileIndex);
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

		void addTileNeighbor(TileIndex tileIndex, const flat::Vector2i& neighborTilePosition);
		void addTileNeighbor(TileIndex tileIndex, TileIndex neighborTileIndex);

		void updateTileNormal(TileIndex tileIndex);
		
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

		// tiles
		std::vector<Tile> m_tiles;
		std::vector<TileNavigation> m_tileNavigations;
		std::vector<TilePosition> m_tilePositions;
		std::vector<NeighborTiles> m_neighborTiles;
		std::unordered_map<flat::Vector2i, TileIndex> m_tilePositionToIndex;
		std::vector<Prop> m_props;

		std::set<TileIndex> m_dirtyNormalTiles;

		std::map<std::string, std::shared_ptr<Zone>> m_zones;

		struct TileSpriteSynchronizer
		{
			std::shared_ptr<const map::TileTemplate> tileTemplate;
			int tileVariantIndex;
			flat::render::SpriteSynchronizer spriteSynchronizer;

			TileSpriteSynchronizer() : tileVariantIndex(0) {}
		};
		std::deque<TileSpriteSynchronizer> m_tileSpriteSynchronizers;

		std::unique_ptr<EntityQuadTree> m_entityQuadtree;
		
	private:
		friend class io::Reader;
};


template <class Func>
void Map::eachTile(Func func) const
{
	for (size_t i = 0, e = m_tiles.size(); i < e; ++i)
	{
		func(static_cast<TileIndex>(i));
	}
}


template <class Func>
void Map::eachNeighborTiles(TileIndex tileIndex, Func func) const
{
	FLAT_ASSERT(m_neighborTiles.size() == m_tiles.size());
	const NeighborTiles& neighborTiles = m_neighborTiles[tileIndex];
	for (int i = 0; i < NeighborTiles::MAX_NEIGHBORS; ++i)
	{
		TileIndex neighborTileIndex = neighborTiles.neighbors[i];
		if (neighborTileIndex == TileIndex::INVALID_TILE)
			break;

		func(neighborTileIndex);
	}
}


template <class Func>
void Map::eachNeighborTilesWithNavigability(TileIndex tileIndex, float jumpHeight, Navigability navigabilityMask, Func func) const
{
	const float maxZ = getTileZ(tileIndex) + jumpHeight;
	eachNeighborTiles(tileIndex, [this, maxZ, navigabilityMask, &func](TileIndex neighborTileIndex)
	{
		if (getTileZ(neighborTileIndex) <= maxZ && isTileNavigable(neighborTileIndex, navigabilityMask))
			func(neighborTileIndex);
	});
}

template <typename Func>
void Map::eachTileEntity(TileIndex tileIndex, Func func) const
{
	FLAT_ASSERT(tileIndex != TileIndex::INVALID_TILE);
	const Tile& tile = m_tiles.at(tileIndex);
	const flat::AABB3& aabb3 = tile.getWorldSpaceAABB();
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


