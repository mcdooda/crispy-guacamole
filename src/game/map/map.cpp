#include <type_traits>

#include "map.h"
#include "tiletemplate.h"
#include "prop.h"
#include "zone.h"
#include "io/reader.h"
#include "io/writer.h"
#include "../mod/mod.h"

namespace game
{
namespace map
{

Map::Map() :
	m_minX(0),
	m_maxX(0),
	m_minY(0),
	m_maxY(0),
	m_displayManager(nullptr)
{
	
}

Map::~Map()
{
	
}

void Map::update(float currentTime)
{
	updateTilesNormals();

	for (TileSpriteSynchronizer& tileSpriteSynchronizer : m_tileSpriteSynchronizers)
	{
		tileSpriteSynchronizer.spriteSynchronizer.update(currentTime);
	}
}

bool Map::load(Game& game, const mod::Mod& mod)
{
	io::Reader reader(game, mod, *this);
	reader.readConfig();

	if (reader.canRead())
	{
		reader.read();
		updateAllTilesNormals();
		return true;
	}
	else
	{
		setBounds(0, 1, 0, 1);
		return false;
	}
}

bool Map::save(const mod::Mod& mod, const std::string& mapName, const std::vector<entity::Entity*>& entities) const
{
	io::Writer writer(mod, mapName, *this);
	if (writer.canWrite())
	{
		writer.write(entities);
		return true;
	}
	return false;
}

void Map::setBounds(int minX, int maxX, int minY, int maxY)
{
	FLAT_ASSERT(minX <= maxX);
	FLAT_ASSERT(minY <= maxY);

	m_minX = minX;
	m_maxX = maxX;
	m_minY = minY;
	m_maxY = maxY;

	flat::AABB2 aabb(
		flat::Vector2(static_cast<float>(minX) - 1.f, static_cast<float>(minY) - 1.f),
		flat::Vector2(static_cast<float>(maxX) + 1.f, static_cast<float>(maxY) + 1.f)
	);
	m_entityQuadtree = std::move(std::make_unique<flat::geometry::QuadTree<entity::Entity, 10, getEntityAABB>>(aabb));

	int maxTileCount = (m_maxX - m_minX + 1) * (m_maxY - m_minY + 1);
	m_tiles.reserve(maxTileCount);
}

void Map::getBounds(int& minX, int& maxX, int& minY, int& maxY) const
{
	minX = m_minX;
	maxX = m_maxX;
	minY = m_minY;
	maxY = m_maxY;
}

void Map::getActualBounds(int& minX, int& maxX, int& minY, int& maxY) const
{
	// note that min and max are swapped
	getBounds(maxX, minX, maxY, minY);

	eachTile([&](TileIndex tileIndex)
	{
		const flat::Vector2i& xy = m_tiles[tileIndex].getXY();
		minX = std::min(minX, xy.x);
		maxX = std::max(maxX, xy.x);
		minY = std::min(minY, xy.y);
		maxY = std::max(maxY, xy.y);
	});
}

TileIndex Map::createTile(int x, int y, float z, flat::render::SpriteSynchronizer& spriteSynchronizer)
{
	TileIndex tileIndex = static_cast<TileIndex>(m_tiles.size());
	Tile& tile = m_tiles.emplace_back();
	tile.synchronizeSpriteTo(*this, spriteSynchronizer);
	tile.setCoordinates(*this, flat::Vector2i(x, y), z, true);
	size_t positionHash = getTileHashFromPosition(x, y);
	FLAT_ASSERT(m_tilePositionToIndex.find(positionHash) == m_tilePositionToIndex.end());
	m_tilePositionToIndex[positionHash] = tileIndex;
	return tileIndex;
}


void Map::deleteTile(Tile* tile)
{

}

void Map::deleteTile(TileIndex tileIndex)
{

}

size_t Map::getTileHashFromPosition(int x, int y) const
{
	return x * 1'000'000 + y;
}

const Tile* Map::getTileByIndex(TileIndex tileIndex) const
{
	return &m_tiles.at(tileIndex);
}


game::map::Tile* Map::getTileByIndex(TileIndex tileIndex)
{
	return &m_tiles.at(tileIndex);
}

const Tile* Map::getTile(int x, int y) const
{
	return const_cast<Map*>(this)->getTile(x, y);
}

Tile* Map::getTile(int x, int y)
{
	TileIndex tileIndex = getTileIndex(x, y);
	if (tileIndex != TileIndex::INVALID)
	{
		return &m_tiles[tileIndex];
	}
	return nullptr;
}

const Tile* Map::getTile(float x, float y) const
{
	return const_cast<Map*>(this)->getTile(x, y);
}

Tile* Map::getTile(float x, float y)
{
	return getTile(static_cast<int>(std::round(x)), static_cast<int>(std::round(y)));
}

TileIndex Map::getTileIndex(int x, int y) const
{
	size_t positionHash = getTileHashFromPosition(x, y);
	std::unordered_map<size_t, TileIndex>::const_iterator it = m_tilePositionToIndex.find(positionHash);
	if (it != m_tilePositionToIndex.end())
	{
		return it->second;
	}
	return TileIndex::INVALID;
}

TileIndex Map::getTileIndex(float x, float y) const
{
	return getTileIndex(static_cast<int>(std::round(x)), static_cast<int>(std::round(y)));
}

TileIndex Map::getTileIndex(const Tile* tile) const
{
	return static_cast<TileIndex>(tile - &m_tiles[0]);
}

const flat::Vector2i& Map::getTileXY(TileIndex tileIndex) const
{
	return m_tiles.at(tileIndex).getXY();
}

void Map::setTileZ(TileIndex tileIndex, float z)
{
	m_tiles[tileIndex].setZ(*this, z);
}


void Map::moveTileZBy(TileIndex tileIndex, float dz)
{
	Tile& tile = m_tiles[tileIndex];
	tile.setZ(*this, tile.getZ() + dz);
}

float Map::getTileZ(TileIndex tileIndex) const
{
	return m_tiles.at(tileIndex).getZ();
}

const Tile* Map::getTileIfNavigable(int x, int y, Navigability navigabilityMask) const
{
	return const_cast<Map*>(this)->getTileIfNavigable(x, y, navigabilityMask);
}

Tile* Map::getTileIfNavigable(int x, int y, Navigability navigabilityMask)
{
	Tile* tile = getTile(x, y);

	if (!tile || !tile->isNavigable(navigabilityMask))
		return nullptr;

	return tile;
}

const Tile* Map::getTileIfNavigable(float x, float y, Navigability navigabilityMask) const
{
	return const_cast<Map*>(this)->getTileIfNavigable(x, y, navigabilityMask);
}

Tile* Map::getTileIfNavigable(float x, float y, Navigability navigabilityMask)
{
	return getTileIfNavigable(static_cast<int>(std::round(x)), static_cast<int>(std::round(y)), navigabilityMask);
}

TileIndex Map::getTileIndexIfNavigable(int x, int y, Navigability navigabilityMask) const
{
	TileIndex tileIndex = getTileIndex(x, y);
	if (tileIndex == TileIndex::INVALID)
	{
		return TileIndex::INVALID;
	}
	const Tile& tile = m_tiles.at(tileIndex);
	if (tile.isNavigable(navigabilityMask))
	{
		return tileIndex;
	}
	return TileIndex::INVALID;
}


TileIndex Map::getTileIndexIfNavigable(float x, float y, Navigability navigabilityMask) const
{
	return getTileIndexIfNavigable(static_cast<int>(std::round(x)), static_cast<int>(std::round(y)), navigabilityMask);
}

bool Map::isTileNavigable(TileIndex tileIndex, Navigability navigabilityMask) const
{
	return m_tiles.at(tileIndex).isNavigable(navigabilityMask);
}

map::Navigability Map::getTileNavigability(TileIndex tileIndex) const
{
	return m_tiles.at(tileIndex).getNavigability();
}

void Map::setTileNavigability(TileIndex tileIndex, Navigability navigability)
{
	m_tiles[tileIndex].setNavigability(navigability);
}

void Map::setTileColor(TileIndex tileIndex, const flat::video::Color& color)
{
	m_tiles[tileIndex].setColor(color);
}

const flat::video::Color& Map::getTileColor(TileIndex tileIndex) const
{
	return m_tiles.at(tileIndex).getColor();
}

void Map::setTilePropTexture(TileIndex tileIndex, std::shared_ptr<const flat::video::Texture> texture)
{
	m_tiles[tileIndex].setPropTexture(*this, texture);
}

void Map::removeTileProp(TileIndex tileIndex)
{
	m_tiles[tileIndex].removeProp(*this);
}

const map::Prop* Map::getTileProp(TileIndex tileIndex) const
{
	return m_tiles.at(tileIndex).getProp();
}

void Map::eachTile(std::function<void(TileIndex)> func) const
{
	for (TileIndex i = static_cast<TileIndex>(0), e = static_cast<TileIndex>(m_tiles.size()); i < e; ++(std::uint32_t&)i)
	{
		func(i);
	}
}

const flat::render::BaseSprite& Map::getTileSprite(TileIndex tileIndex) const
{
	return m_tiles.at(tileIndex).getSprite();
}

void Map::synchronizeTileSpriteTo(TileIndex tileIndex, flat::render::SpriteSynchronizer& synchronizer)
{
	m_tiles[tileIndex].synchronizeSpriteTo(*this, synchronizer);
}

flat::render::SpriteSynchronizer& Map::getTileSpriteSynchronizer(const std::shared_ptr<const TileTemplate>& tileTemplate, int tileVariantIndex)
{
	std::deque<TileSpriteSynchronizer>::iterator it = std::find_if(
		m_tileSpriteSynchronizers.begin(),
		m_tileSpriteSynchronizers.end(),
		[tileTemplate, tileVariantIndex](const TileSpriteSynchronizer& tileSpriteSynchronize)
		{
			return tileSpriteSynchronize.tileTemplate == tileTemplate
				&& tileSpriteSynchronize.tileVariantIndex == tileVariantIndex;
		}
	);

	if (it != m_tileSpriteSynchronizers.end())
	{
		return it->spriteSynchronizer;
	}

	m_tileSpriteSynchronizers.emplace_back();
	TileSpriteSynchronizer& tileSpriteSynchronizer = m_tileSpriteSynchronizers.back();
	tileSpriteSynchronizer.tileTemplate = tileTemplate;
	tileSpriteSynchronizer.tileVariantIndex = tileVariantIndex;

	flat::render::SpriteSynchronizer& spriteSynchronizer = tileSpriteSynchronizer.spriteSynchronizer;
	spriteSynchronizer.setTexture(tileTemplate->getTexture());
	spriteSynchronizer.setAtlasSize(tileTemplate->getNumFrames(), tileTemplate->getNumVariants());
	spriteSynchronizer.playAnimation(
		tileVariantIndex,
		tileTemplate->getNumFrames(),
		tileTemplate->getFrameDuration(),
		flat::render::AnimatedSprite::INFINITE_LOOP
	);

	return spriteSynchronizer;
}

const std::shared_ptr<const TileTemplate> Map::getTileTemplate(const Tile* tile) const
{
	const flat::render::SynchronizedSprite& sprite = static_cast<const flat::render::SynchronizedSprite&>(tile->getSprite());
	flat::render::SpriteSynchronizer& synchronizer = sprite.getSynchronizer();
	for (const TileSpriteSynchronizer& tileSpriteSynchronizer : m_tileSpriteSynchronizers)
	{
		if (&tileSpriteSynchronizer.spriteSynchronizer == &synchronizer)
		{
			return tileSpriteSynchronizer.tileTemplate;
		}
	}
	return nullptr;
}

const std::shared_ptr<const TileTemplate> Map::getTileTemplate(TileIndex tileIndex) const
{
	return getTileTemplate(&m_tiles.at(tileIndex));
}

int Map::addEntity(entity::Entity* entity)
{
	return m_entityQuadtree->addObject(entity);
}

void Map::removeEntity(entity::Entity* entity, int cellIndex)
{
	m_entityQuadtree->removeObject(entity, cellIndex);
}

int Map::updateEntityPosition(entity::Entity* entity, int cellIndex)
{
	return m_entityQuadtree->updateObject(entity, cellIndex);
}

int Map::getTileEntityCount(TileIndex tileIndex) const
{
	int entityCount = 0;
	eachTileEntity(
		tileIndex,
		[&entityCount](entity::Entity* entity)
		{
			++entityCount;
		}
	);
	return entityCount;
}

void Map::eachNeighborTilesWithNavigability(TileIndex tileIndex, float jumpHeight, Navigability navigabilityMask, std::function<void(TileIndex)> func) const
{
	m_tiles.at(tileIndex).eachNeighborTilesWithNavigability(*this, jumpHeight, navigabilityMask, func);
}

void Map::setTileNormalDirty(Tile& tile)
{
	if (!tile.isNormalDirty())
	{
		tile.setNormalDirty();
		m_dirtyNormalTiles.push_back(&tile);
	}
}

void Map::updateTilesNormals()
{
	for (Tile* tile : m_dirtyNormalTiles)
	{
		tile->updateNormal(*this);
	}
	m_dirtyNormalTiles.clear();
}

void Map::updateAllTilesNormals()
{
	m_dirtyNormalTiles.clear();
	eachTile([this](TileIndex tileIndex)
	{
		m_tiles[tileIndex].updateNormal(*this);
	});
}

std::shared_ptr<Zone>& Map::addZone(const std::string& zoneName)
{
	FLAT_ASSERT(m_zones.find(zoneName) == m_zones.end());
	return m_zones.emplace(zoneName, std::make_shared<Zone>(*this)).first->second;
}

bool Map::removeZone(const std::string& zoneName)
{
	std::map<std::string, std::shared_ptr<Zone>>::iterator it = m_zones.find(zoneName);
	if (it != m_zones.end())
	{
		m_zones.erase(it);
		return true;
	}
	return false;
}

bool Map::getZone(const std::string& zoneName, std::shared_ptr<Zone>& zone) const
{
	std::map<std::string, std::shared_ptr<Zone>>::const_iterator it = m_zones.find(zoneName);
	if (it != m_zones.end())
	{
		zone = it->second;
		return true;
	}
	return false;
}

void Map::setAxes(const flat::Vector2& xAxis,
                  const flat::Vector2& yAxis,
                  const flat::Vector2& zAxis)
{
	m_transform = flat::Matrix3(flat::Vector3(xAxis, 1.f), flat::Vector3(yAxis, 1.f), flat::Vector3(zAxis, 1.f));
	m_invTransform = flat::inverse(m_transform);
	m_xAxis = xAxis;
	m_yAxis = yAxis;
	m_zAxis = zAxis;
}

} // map
} // game



