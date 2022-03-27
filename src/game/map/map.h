#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <deque>
#include <functional>
#include <unordered_set>

#include <flat.h>

#include "map/tile.h"
#include "map/prop.h"
#include "map/fog/fog.h"
#include "map/io/mapfile.h"

#include "entity/entity.h"
#include "entity/entityhelper.h"

#include "debug/debugdisplay.h"

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
class Writer;
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

	public:
		Map();
		Map(const Map&) = delete;
		Map(Map&&) = delete;
		~Map() = default;

		Map& operator=(const Map&) = delete;
		Map& operator=(Map&&) = delete;

		void reset(Game& game, int width, int height, const std::shared_ptr<const TileTemplate>& tileTemplate);

		void setState(Game& game, const io::MapFile& mapFile);
		void getState(Game& game, io::MapFile& mapFile) const;

		void update(Game& game, float currentTime);

		void setDisplayManager(DisplayManager* displayManager) { m_displayManager = displayManager; }
		inline DisplayManager& getDisplayManager() const { FLAT_ASSERT(m_displayManager != nullptr); return *m_displayManager; }

		void addAllTilesToDisplayManager() const;
		void addAllPropsToDisplayManager() const;

		bool load(Game& game);
		bool save(Game& game) const;

		void setBounds(int minX, int maxX, int minY, int maxY);
		void getBounds(int& minX, int& maxX, int& minY, int& maxY) const;
		void getActualBounds(int& minX, int& maxX, int& minY, int& maxY) const;

		// tiles
		TileIndex createTile(const flat::Vector2i& xy, float z, const std::shared_ptr<const TileTemplate>& tileTemplate, uint16_t tileTemplateVariantIndex);
		void deleteTile(TileIndex tileIndex);
		void deleteTile(const flat::Vector2i& tilePosition);

		void setTileTemplate(TileIndex tileIndex, const std::shared_ptr<const TileTemplate>& tileTemplate);
		void setTileTemplateVariant(TileIndex tileIndex, uint16_t tileTemplateVariantIndex);

		TileIndex getTileIndex(int x, int y) const;
		TileIndex getTileIndex(const flat::Vector2i& position) const;
		inline TileIndex getTileIndex(float x, float y) const { return getTileIndex(static_cast<int>(std::floor(x + 0.5f)), static_cast<int>(std::floor(y + 0.5f))); }
		inline TileIndex getTileIndex(const flat::Vector2& position) const { return getTileIndex(position.x, position.y); }
		TileIndex getTileIndex(const Tile* tile) const;
		std::vector<TileIndex> getTilesIndices(const std::vector<flat::Vector2>& positions) const;
		std::vector<flat::Vector2> getTilesPositions(const std::vector<TileIndex>& indices) const;

		void getTilesFromIndices(const std::vector<TileIndex>& tileIndices, std::vector<const Tile*>& tiles) const;

		const Tile& getTileFromIndex(TileIndex tileIndex) const;

		int getTilesCount() const { return static_cast<int>(m_tiles.size()); }
		int getPropsCount() const { return static_cast<int>(m_props.size()); }

		void getPropsFromIndices(const std::vector<PropIndex>& propIndices, std::vector<const Prop*>& props) const;

		const flat::Vector2i& getTileXY(TileIndex tileIndex) const;
		void setTileZ(TileIndex tileIndex, float z);
		void moveTileZBy(TileIndex tileIndex, float dz);
		float getTileZ(TileIndex tileIndex) const;
		const flat::AABB3& getTileAABB(TileIndex tileIndex) const;

		TileIndex findTileIndexIfNavigable(const flat::Vector2& xy, Navigability navigabilityMask) const;

		bool isTileNavigable(TileIndex tileIndex, Navigability navigabilityMask) const;
		Navigability getTileNavigability(TileIndex tileIndex) const;
		void setTileNavigability(TileIndex tileIndex, Navigability navigability);
		void resetTileNavigabilityFromTemplate(TileIndex tileIndex);

#ifdef FLAT_DEBUG
		void enableNavigabilityDebug(bool enable);
		void updateTileNavigabilityDebug(TileIndex tileIndex);
#endif

		void setTileColor(TileIndex tileIndex, const flat::video::Color& color, bool updatePropColor = true);
		const flat::video::Color& getTileColor(TileIndex tileIndex) const;

		void setTilePropTexture(TileIndex tileIndex, const std::shared_ptr<const flat::video::Texture>& texture);
		void removeTileProp(TileIndex tileIndex);
		const Prop* getTileProp(TileIndex tileIndex) const;
		PropIndex getTilePropIndex(TileIndex tileIndex) const;
		const Prop& getPropFromIndex(PropIndex propIndex) const;

		const flat::render::BaseSprite* getTileSprite(TileIndex tileIndex) const;
		flat::render::BaseSprite* getTileSprite(TileIndex tileIndex);

		const flat::render::Mesh* getTileMesh(TileIndex tileIndex) const;
		flat::render::Mesh* getTileMesh(TileIndex tileIndex);

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

		template <class Func>
		inline void eachEntityInRange(const flat::Vector2& center2d, float range, Func func) const;

		template <class Func>
		inline void eachEntityInCollisionRange(const flat::Vector2& center2d, float range, Func func) const;

		void setTileNormalDirty(TileIndex tileIndex);
		void setTileTextureDirty(TileIndex tileIndex);

		void updateDirtyTiles(Game& game);
		void updateAllTiles();

		std::shared_ptr<Zone>& addZone(const std::string& zoneName);
		bool removeZone(const std::string& zoneName);
		bool getZone(const std::string& zoneName, std::shared_ptr<Zone>& zone) const;
		inline const std::map<std::string, std::shared_ptr<Zone>>& getZones() const { return m_zones; }

		bool straightPathExists(const flat::Vector2& from, const flat::Vector2& to, float jumpHeight, Navigability navigability) const;
		bool navigationRaycast(const flat::Vector2& startPosition, const flat::Vector2& direction, float length, float jumpHeight, Navigability navigabilityMask, flat::Vector2& endPosition) const;

		void setFogType(fog::Fog::FogType fogType);
		fog::Fog::FogType getFogType() const;

		inline fog::Fog& getFog() { return *m_fog; }

		bool isOnTiles(const flat::AABB2& aabb) const;

		inline bool isLoaded() const { return m_isLoaded; }

#ifdef FLAT_DEBUG
		void enableTileIndicesDebug(bool enable) { m_enableTileIndicesDebug = enable; }
		void debugDraw(debug::DebugDisplay& debugDisplay) const;
#endif // FLAT_DEBUG

		inline const flat::sharp::ai::navigation::AStarGridNavigationData& getNavigationGrid() const { return m_navigationGrid; }

	protected:
		void setAxes(const flat::Vector2& xAxis,
		             const flat::Vector2& yAxis,
		             const flat::Vector2& zAxis);
		void getAxes(flat::Vector2& xAxis,
					 flat::Vector2& yAxis,
					 flat::Vector2& zAxis) const;

		void updateTileNormal(TileIndex tileIndex);
		void updateTileTexture(Game& game, TileIndex tileIndex);

	protected:
		DisplayManager* m_displayManager;

		std::unique_ptr<map::fog::Fog> m_fog;

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
		flat::sharp::ai::navigation::AStarGridNavigationData m_navigationGrid;

		std::vector<Tile> m_tiles;
		std::vector<Prop> m_props;

		std::unordered_set<TileIndex> m_dirtyNormalTiles;
		std::unordered_set<TileIndex> m_dirtyTextureTiles;

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

		bool m_isLoaded;

#ifdef FLAT_DEBUG
		bool m_enableTileIndicesDebug;
		bool m_enableNavigabilityDebug;
#endif

	private:
		friend class io::Reader;
		friend class io::Writer;
};


template <class Func>
void Map::eachTile(Func func) const
{
	for (size_t i = 0, e = m_tiles.size(); i < e; ++i)
	{
		func(static_cast<TileIndex>(i));
	}
}

template <typename Func>
void Map::eachTileEntity(TileIndex tileIndex, Func func) const
{
	FLAT_ASSERT(isValidTile(tileIndex));
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


