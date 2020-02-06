#include <type_traits>

#include "map/map.h"
#include "map/tiletemplate.h"
#include "map/prop.h"
#include "map/zone.h"
#include "map/io/reader.h"
#include "map/io/writer.h"
#include "map/displaymanager.h"
#include "map/fog/fog.h"

#include "mod/mod.h"

#include "states/basemapstate.h"

#include "game.h"

namespace game
{
namespace map
{

Map::Map() :
	m_minX(0),
	m_maxX(0),
	m_minY(0),
	m_maxY(0),
	m_displayManager(nullptr),
	m_isLoaded(false)
{
#ifdef FLAT_DEBUG
	m_enableTileIndicesDebug = false;
	m_enableNavigabilityDebug = false;
#endif
}

Map::~Map()
{
	
}

void Map::setState(Game& game, const mod::Mod& mod, const io::MapFile& mapFile)
{
	states::BaseMapState& baseMapState = game.getStateMachine().getState()->to<states::BaseMapState>();

	setAxes(mapFile.getXAxis(), mapFile.getYAxis(), mapFile.getZAxis());
	setBounds(mapFile.getMinX(), mapFile.getMaxX(), mapFile.getMinY(), mapFile.getMaxY());

	// tiles
	m_tiles.reserve(mapFile.getTilesCount());
	mapFile.eachTile(
		[this, &game, &mod, &baseMapState]
		(
			const flat::Vector2i& tilePosition,
			const io::MapFile::Tile& tile,
			const std::string& tileTemplateName,
			const std::string* propTemplateName
		)
	{
		const std::shared_ptr<const TileTemplate> tileTemplate = baseMapState.getTileTemplate(game, tileTemplateName);
		TileIndex tileIndex = createTile(tilePosition, tile.z, tileTemplate, tile.tileTemplateVariantIndex);
		if (propTemplateName != nullptr)
		{
			const std::string texturePath = mod.getTexturePath("props/" + *propTemplateName);
			const std::shared_ptr<const flat::video::FileTexture>& texture = game.video->getTexture(texturePath);
			setTilePropTexture(tileIndex, texture);
		}
	});
	updateAllTiles();

	// entities
	std::vector<std::shared_ptr<const entity::EntityTemplate>> entityTemplates;
	entityTemplates.reserve(mapFile.getEntityTemplates().size());
	for (const std::string& entityTemplateName : mapFile.getEntityTemplates())
	{
		const std::shared_ptr<const entity::EntityTemplate> entityTemplate = baseMapState.getEntityTemplate(game, entityTemplateName);
		entityTemplates.push_back(entityTemplate);
	}

	for (const io::MapFile::Entity& entity : mapFile.getEntities())
	{
		const std::shared_ptr<const entity::EntityTemplate> entityTemplate = entityTemplates[entity.entityTemplateIndex];
		if (entityTemplate->getComponentFlags() != 0)
		{
			const TileIndex tileIndex = getTileIndex(entity.position.x, entity.position.y);
			FLAT_ASSERT(isValidTile(tileIndex));
			const flat::Vector3 position(entity.position.x, entity.position.y, getTileZ(tileIndex));
			baseMapState.spawnEntityAtPosition(game, entityTemplate, position);
		}
		else
		{
			std::cerr << "Entity '" << entityTemplate->getName() << "' does not exist anymore" << std::endl;
		}
	}

	// zones
	for (const io::MapFile::Zone& fileZone : mapFile.getZones())
	{
		std::shared_ptr<Zone> zone = addZone(fileZone.name);
		for (const io::MapFile::Zone::Rectangle& fileRectangle : fileZone.rectangles)
		{
			Zone::Rectangle rectangle;
			rectangle.minX = fileRectangle.minX;
			rectangle.minY = fileRectangle.minY;
			rectangle.maxX = fileRectangle.maxX;
			rectangle.maxY = fileRectangle.maxY;
			zone->addRectangle(rectangle);
		}
	}
}

void Map::getState(io::MapFile& mapFile) const
{

}

void Map::update(float currentTime)
{
	updateDirtyTiles();

	for (TileSpriteSynchronizer& tileSpriteSynchronizer : m_tileSpriteSynchronizers)
	{
		tileSpriteSynchronizer.spriteSynchronizer.update(currentTime);
	}
}

void Map::addAllTilesToDisplayManager() const
{
	for (int i = 0, e = getTilesCount(); i < e; ++i)
	{
		const TileIndex tileIndex = static_cast<TileIndex>(i);
		m_displayManager->addTile(tileIndex, &m_tiles[tileIndex]);
	}
}

void Map::addAllPropsToDisplayManager() const
{
	for (int i = 0, e = getPropsCount(); i < e; ++i)
	{
		const PropIndex propIndex = static_cast<PropIndex>(i);
		m_displayManager->addProp(propIndex, &m_props[propIndex]);
	}
}

bool Map::load(Game& game, const mod::Mod& mod)
{
	FLAT_ASSERT(!m_isLoaded);

	io::Reader reader(game, mod, *this);

	bool loadedFromFile = false;
	if (reader.canRead())
	{
		reader.read();
		loadedFromFile = true;
	}
	else
	{
		setBounds(0, 1, 0, 1);
		setAxes(
			flat::Vector2(-20.f, 10.f),
			flat::Vector2(20.f, 10.f),
			flat::Vector2(0, -20.f)
		);
	}
	m_isLoaded = true;
	setFogType(fog::Fog::FogType::NONE);
	return loadedFromFile;
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

	m_entityQuadtree = std::make_unique<EntityQuadTree>(aabb);

	int maxTileCount = (m_maxX - m_minX + 1) * (m_maxY - m_minY + 1);
	m_tiles.reserve(maxTileCount);
	m_tileNavigations.reserve(maxTileCount);
	m_tilePositions.reserve(maxTileCount);
	m_props.reserve(maxTileCount);
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
		const flat::Vector2i& xy = getTileXY(tileIndex);
		minX = std::min(minX, xy.x);
		maxX = std::max(maxX, xy.x);
		minY = std::min(minY, xy.y);
		maxY = std::max(maxY, xy.y);
	});
}

TileIndex Map::createTile(const flat::Vector2i& xy, float z, const std::shared_ptr<const TileTemplate>& tileTemplate, uint16_t tileTemplateVariantIndex)
{
	const TileIndex tileIndex = static_cast<TileIndex>(m_tiles.size());

	Tile& tile = m_tiles.emplace_back();
	flat::render::SpriteSynchronizer& spriteSynchronizer = getTileSpriteSynchronizer(tileTemplate, tileTemplateVariantIndex);
	tile.synchronizeSpriteTo(*this, spriteSynchronizer);

	// move sprite and update aabb
	flat::Vector3 position(xy.x, xy.y, z);
	flat::Vector2 position2d(getTransform() * position);
	tile.setSpritePosition(position2d);
	tile.updateWorldSpaceAABB(position);

	// navigation
	FLAT_ASSERT(m_tilePositionToIndex.find(xy) == m_tilePositionToIndex.end());
	m_tilePositionToIndex[xy] = tileIndex;

	TileNavigation& tileNavigation = m_tileNavigations.emplace_back();
	tileNavigation.z = z;
	tileNavigation.navigability = tileTemplate->getNavigability();

	TilePosition& tilePosition = m_tilePositions.emplace_back();
	tilePosition.xy = xy;

	setTileDirty(tileIndex);

	if (m_fog != nullptr)
	{
		m_fog->addTile(tileIndex, &tile);
	}

	return tileIndex;
}

void Map::deleteTile(TileIndex tileIndex)
{
	FLAT_ASSERT(tileIndex < m_tiles.size());

#ifdef FLAT_DEBUG
	//checkTilePropIndicesIntegrity();
#endif

	removeTileProp(tileIndex);

	TilePosition tilePosition = m_tilePositions.at(tileIndex);
	if (tileIndex == m_tiles.size() - 1)
	{
		m_tiles.pop_back();
		m_fog->removeTile(tileIndex);
		m_tilePositionToIndex.erase(tilePosition.xy);
		m_tileNavigations.pop_back();
		m_tilePositions.pop_back();
	}
	else
	{
		TileIndex movedTileIndex = static_cast<TileIndex>(m_tiles.size() - 1);

		PropIndex movedTilePropIndex = m_tiles[movedTileIndex].getPropIndex();
		if (movedTilePropIndex != PropIndex::INVALID_PROP)
		{
			FLAT_ASSERT(m_props[movedTilePropIndex].getTileIndex() == movedTileIndex);
			m_props[movedTilePropIndex].setTileIndex(tileIndex);
		}

		TilePosition movedTilePosition = m_tilePositions.at(movedTileIndex);
		m_tiles[tileIndex] = std::move(m_tiles[movedTileIndex]);
		m_fog->removeTile(tileIndex);
		m_fog->moveTileIndex(movedTileIndex, tileIndex);
		m_tiles.pop_back();
		m_tilePositionToIndex[movedTilePosition.xy] = tileIndex;
		m_tilePositionToIndex.erase(tilePosition.xy);
		m_tileNavigations[tileIndex] = m_tileNavigations[movedTileIndex];
		m_tileNavigations.pop_back();
		m_tilePositions[tileIndex] = m_tilePositions[movedTileIndex];
		m_tilePositions.pop_back();
	}

#ifdef FLAT_DEBUG
	//checkTilePropIndicesIntegrity();
#endif
}

void Map::deleteTile(const flat::Vector2i& tilePosition)
{
	deleteTile(m_tilePositionToIndex.at(tilePosition));
}

void Map::replaceTile(TileIndex tileIndex, const std::shared_ptr<const TileTemplate>& tileTemplate, uint16_t tileTemplateVariantIndex)
{
	flat::render::SpriteSynchronizer& synchronizer = getTileSpriteSynchronizer(tileTemplate, tileTemplateVariantIndex);
	Tile& tile = m_tiles[tileIndex];
	tile.synchronizeSpriteTo(*this, synchronizer);
	m_fog->updateTile(tileIndex, &tile);

	TileNavigation& tileNavigation = m_tileNavigations[tileIndex];
	tileNavigation.navigability = tileTemplate->getNavigability();
}

TileIndex Map::getTileIndex(int x, int y) const
{
	return getTileIndex(flat::Vector2i(x, y));
}

TileIndex Map::getTileIndex(const flat::Vector2i& position) const
{
	std::unordered_map<flat::Vector2i, TileIndex>::const_iterator it = m_tilePositionToIndex.find(position);
	if (it != m_tilePositionToIndex.end())
	{
		return it->second;
	}
	return TileIndex::INVALID_TILE;
}

TileIndex Map::getTileIndex(const Tile* tile) const
{
	FLAT_ASSERT(&m_tiles[0] <= tile && tile <= &m_tiles[m_tiles.size() - 1]);
	return static_cast<TileIndex>(tile - &m_tiles[0]);
}

void Map::getTilesFromIndices(const std::vector<TileIndex>& tileIndices, std::vector<const Tile*>& tiles) const
{
	FLAT_PROFILE("Map Get Tiles From Indices");

	tiles.reserve(tileIndices.size());
	for (TileIndex tileIndex : tileIndices)
	{
		tiles.push_back(&m_tiles[tileIndex]);
	}
}

const Tile& Map::getTileFromIndex(TileIndex tileIndex) const
{
	return m_tiles[tileIndex];
}

void Map::getPropsFromIndices(const std::vector<PropIndex>& propIndices, std::vector<const Prop*>& props) const
{
	FLAT_PROFILE("Map Get Props From Indices");

	props.reserve(propIndices.size());
	for (PropIndex propIndex : propIndices)
	{
		props.push_back(&m_props[propIndex]);
	}
}

const flat::Vector2i& Map::getTileXY(TileIndex tileIndex) const
{
	return m_tilePositions[tileIndex].xy;
}

void Map::setTileZ(TileIndex tileIndex, float z)
{
	// navigation
	m_tileNavigations[tileIndex].z = z;

	// move sprite and update aabb
	const flat::Vector2i& xy = getTileXY(tileIndex);
	Tile& tile = m_tiles[tileIndex];
	flat::Vector3 position(xy.x, xy.y, z);
	flat::Vector2 position2d(getTransform() * position);
	tile.setSpritePosition(position2d);
	tile.updateWorldSpaceAABB(position);
	m_fog->updateTile(tileIndex, &tile);

	// prop
	PropIndex propIndex = tile.getPropIndex();
	if (propIndex != PropIndex::INVALID_PROP)
	{
		Prop& prop = m_props[propIndex];
		prop.setSpritePosition(position2d);
		prop.updateWorldSpaceAABB(position);
		m_fog->updateProp(propIndex, &prop);
	}

	setTileDirty(tileIndex);
}

void Map::moveTileZBy(TileIndex tileIndex, float dz)
{
	setTileZ(tileIndex, m_tileNavigations[tileIndex].z + dz);
}

float Map::getTileZ(TileIndex tileIndex) const
{
	return m_tileNavigations[tileIndex].z;
}

const flat::AABB3& Map::getTileAABB(TileIndex tileIndex) const
{
	return m_tiles[tileIndex].getWorldSpaceAABB();
}

TileIndex Map::getTileIndexIfNavigable(int x, int y, Navigability navigabilityMask) const
{
	TileIndex tileIndex = getTileIndex(x, y);
	if (tileIndex == TileIndex::INVALID_TILE)
	{
		return TileIndex::INVALID_TILE;
	}
	if (isTileNavigable(tileIndex, navigabilityMask))
	{
		return tileIndex;
	}
	return TileIndex::INVALID_TILE;
}


TileIndex Map::getTileIndexIfNavigable(float x, float y, Navigability navigabilityMask) const
{
	return getTileIndexIfNavigable(static_cast<int>(std::round(x)), static_cast<int>(std::round(y)), navigabilityMask);
}

bool Map::isTileNavigable(TileIndex tileIndex, Navigability navigabilityMask) const
{
	return (m_tileNavigations[tileIndex].navigability & navigabilityMask) != 0;
}

Navigability Map::getTileNavigability(TileIndex tileIndex) const
{
	return m_tileNavigations[tileIndex].navigability;
}

void Map::setTileNavigability(TileIndex tileIndex, Navigability navigability)
{
	m_tileNavigations[tileIndex].navigability = navigability;
#ifdef FLAT_DEBUG
	if (m_enableNavigabilityDebug)
	{
		updateTileNavigabilityDebug(tileIndex);
	}
#endif // FLAT_DEBUG
}

void Map::resetTileNavigabilityFromTemplate(TileIndex tileIndex)
{
	Navigability navigability = getTileTemplate(tileIndex)->getNavigability();
	setTileNavigability(tileIndex, navigability);
}

#ifdef FLAT_DEBUG
void Map::enableNavigabilityDebug(bool enable)
{
	m_enableNavigabilityDebug = enable;
	if (m_enableNavigabilityDebug)
	{
		for (int i = 0, e = getTilesCount(); i < e; ++i)
		{
			const TileIndex tileIndex = static_cast<TileIndex>(i);
			updateTileNavigabilityDebug(tileIndex);
		}
	}
	else
	{
		for (int i = 0, e = getTilesCount(); i < e; ++i)
		{
			const TileIndex tileIndex = static_cast<TileIndex>(i);
			setTileColor(tileIndex, flat::video::Color::WHITE, false);
		}
	}
}

void Map::updateTileNavigabilityDebug(TileIndex tileIndex)
{
	Navigability navigability = m_tileNavigations[tileIndex].navigability;
	flat::video::Color color = flat::video::Color::WHITE;
	switch (navigability)
	{
	case Navigability::NONE:
		color = flat::video::Color::BLACK;
		break;
	case Navigability::GROUND:
		color = flat::video::Color::GREEN;
		break;
	case Navigability::WATER:
		color = flat::video::Color::BLUE;
		break;
	case Navigability::ALL:
		color = flat::video::Color::RED;
		break;
	}
	setTileColor(tileIndex, color, false);
}
#endif

void Map::setTileColor(TileIndex tileIndex, const flat::video::Color& color, bool updatePropColor)
{
	Tile& tile = m_tiles[tileIndex];
	tile.getSprite().setColor(color);
	m_fog->updateTile(tileIndex, &tile);

	if (updatePropColor)
	{
		const PropIndex propIndex = m_tiles[tileIndex].getPropIndex();
		if (isValidProp(propIndex))
		{
			Prop& prop = m_props[propIndex];
			prop.setSpriteColor(color);
			m_fog->updateProp(propIndex, &prop);
		}
	}
}

const flat::video::Color& Map::getTileColor(TileIndex tileIndex) const
{
	return m_tiles[tileIndex].getSprite().getColor();
}

void Map::setTilePropTexture(TileIndex tileIndex, std::shared_ptr<const flat::video::Texture> texture)
{
	Tile& tile = m_tiles[tileIndex];
	PropIndex propIndex = tile.getPropIndex();
	bool isNewProp;
	Prop* prop = nullptr;
	if (propIndex == PropIndex::INVALID_PROP)
	{
		propIndex = static_cast<PropIndex>(m_props.size());
		isNewProp = true;
		prop = &m_props.emplace_back();
		prop->setSpritePosition(tile.getSprite().getPosition());
		const flat::Vector2i& xy = getTileXY(tileIndex);
		const float z = getTileZ(tileIndex);
		prop->updateWorldSpaceAABB(flat::Vector3(xy.x, xy.y, z));
		tile.setPropIndex(propIndex);

		// remove all navigability
		setTileNavigability(tileIndex, Navigability::NONE);
	}
	else
	{
		isNewProp = false;
		prop = &m_props[propIndex];
	}

	prop->setTileIndex(tileIndex);
	prop->setSpriteTexture(texture);
	const flat::Vector2& textureSize = texture->getSize();
	flat::Vector2 origin(textureSize.x / 2.f, textureSize.y - textureSize.x / 4.f);
	prop->setSpriteOrigin(origin);
	prop->updateRenderHash();

	FLAT_ASSERT(prop->getAABB().isValid());

	if (isNewProp)
	{
		if (m_fog != nullptr) // fog is null during loading
		{
			m_fog->addProp(propIndex, prop);
		}
	}
	else
	{
		m_fog->updateProp(propIndex, prop);
	}

#ifdef FLAT_DEBUG
	//checkTilePropIndicesIntegrity();
#endif
}

void Map::removeTileProp(TileIndex tileIndex)
{
	Tile& tile = m_tiles[tileIndex];
	PropIndex propIndex = tile.getPropIndex();
	if (propIndex == PropIndex::INVALID_PROP)
	{
		return;
	}

	resetTileNavigabilityFromTemplate(tileIndex);

	tile.setPropIndex(PropIndex::INVALID_PROP);

	if (propIndex == m_props.size() - 1)
	{
		m_props.pop_back();
		m_fog->removeProp(propIndex);
	}
	else
	{
		PropIndex movedPropIndex = static_cast<PropIndex>(m_props.size() - 1);

		TileIndex movedPropTileIndex = m_props[movedPropIndex].getTileIndex();
		FLAT_ASSERT(m_tiles[movedPropTileIndex].getPropIndex() == movedPropIndex);
		m_tiles[movedPropTileIndex].setPropIndex(propIndex);

		m_props[propIndex] = std::move(m_props[movedPropIndex]);
		m_fog->removeProp(propIndex);
		m_fog->movePropIndex(movedPropIndex, propIndex);
		m_props.pop_back();
	}

#ifdef FLAT_DEBUG
		//checkTilePropIndicesIntegrity();
#endif
}

const Prop* Map::getTileProp(TileIndex tileIndex) const
{
	PropIndex propIndex = getTilePropIndex(tileIndex);
	if (isValidProp(propIndex))
	{
		return &m_props[propIndex];
	}
	return nullptr;
}

PropIndex Map::getTilePropIndex(TileIndex tileIndex) const
{
	return m_tiles[tileIndex].getPropIndex();
}

const Prop& Map::getPropFromIndex(PropIndex propIndex) const
{
	return m_props[propIndex];
}

const flat::render::BaseSprite& Map::getTileSprite(TileIndex tileIndex) const
{
	return m_tiles[tileIndex].getSprite();
}

flat::render::BaseSprite& Map::getTileSprite(TileIndex tileIndex)
{
	return m_tiles[tileIndex].getSprite();
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

const std::shared_ptr<const TileTemplate> Map::getTileTemplate(TileIndex tileIndex) const
{
	const Tile& tile = m_tiles[tileIndex];
	const flat::render::SynchronizedSprite& sprite = static_cast<const flat::render::SynchronizedSprite&>(tile.getSprite());
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

#ifdef FLAT_DEBUG
void Map::checkTilePropIndicesIntegrity() const
{
	for (int i = 0; i < m_tiles.size(); ++i)
	{
		PropIndex propIndex = m_tiles[i].getPropIndex();
		if (propIndex != PropIndex::INVALID_PROP)
		{
			FLAT_ASSERT(m_props[propIndex].getTileIndex() == i);
		}
	}

	for (int i = 0; i < m_props.size(); ++i)
	{
		TileIndex tileIndex = m_props[i].getTileIndex();
		if (tileIndex != TileIndex::INVALID_TILE)
		{
			FLAT_ASSERT(m_tiles[tileIndex].getPropIndex() == i);
		}
	}
}
#endif // FLAT_DEBUG

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

void Map::addTileNeighbor(TileIndex tileIndex, TileIndex neighborTileIndex)
{
	NeighborTiles& neighborTiles = m_neighborTiles[tileIndex];
	for (size_t i = 0; i < NeighborTiles::MAX_NEIGHBORS; ++i)
	{
		if (neighborTiles.neighbors[i] == TileIndex::INVALID_TILE)
		{
			neighborTiles.neighbors[i] = neighborTileIndex;

			for (size_t j = i; j > 0 && neighborTiles.neighbors[j] < neighborTiles.neighbors[j - 1]; --j)
			{
				std::swap(neighborTiles.neighbors[j], neighborTiles.neighbors[j - 1]);
			}
			return;
		}
	}
	FLAT_ASSERT_MSG(false, "Cannot add more than %d neighbors", NeighborTiles::MAX_NEIGHBORS);
}

void Map::addTileNeighbor(TileIndex tileIndex, const flat::Vector2i& neighborTilePosition)
{
	TileIndex neighborTileIndex = getTileIndex(neighborTilePosition);
	if (neighborTileIndex != TileIndex::INVALID_TILE)
	{
		addTileNeighbor(tileIndex, neighborTileIndex);
	}
}

void Map::updateTileNormal(TileIndex tileIndex)
{
	flat::Vector3 dx(1.f, 0.f, 0.f);
	flat::Vector3 dy(0.f, 1.f, 0.f);

	constexpr float minZDifference = 0.05f;

	const flat::Vector2i& xy = getTileXY(tileIndex);
	const float z = getTileZ(tileIndex);

	// compute dx
	{
		TileIndex bottomLeftTileIndex = getTileIndex(xy.x + 1, xy.y);
		TileIndex topRightTileIndex = getTileIndex(xy.x - 1, xy.y);
		if (bottomLeftTileIndex != TileIndex::INVALID_TILE && topRightTileIndex != TileIndex::INVALID_TILE)
		{
			const float bottomLeftTileZ = getTileZ(bottomLeftTileIndex);
			const float topRightTileZ = getTileZ(topRightTileIndex);
			if (std::abs(z - bottomLeftTileZ) > minZDifference
				&& std::abs(z - topRightTileZ) > minZDifference)
			{
				dx.x = 2.f;
				dx.y = 0.f;
				dx.z = bottomLeftTileZ - topRightTileZ;
			}
		}
		else if (bottomLeftTileIndex != TileIndex::INVALID_TILE)
		{
			const float bottomLeftTileZ = getTileZ(bottomLeftTileIndex);
			if (std::abs(z - bottomLeftTileZ) > minZDifference)
			{
				dx.x = 1.f;
				dx.y = 0.f;
				dx.z = bottomLeftTileZ - z;
			}
		}
		else if (topRightTileIndex != TileIndex::INVALID_TILE)
		{
			const float topRightTileZ = getTileZ(topRightTileIndex);
			if (std::abs(z - topRightTileZ) > minZDifference)
			{
				dx.x = 1.f;
				dx.y = 0.f;
				dx.z = z - topRightTileZ;
			}
		}
	}

	// compute dy
	{
		TileIndex bottomRightTileIndex = getTileIndex(xy.x, xy.y + 1);
		TileIndex topLeftTileIndex = getTileIndex(xy.x, xy.y - 1);
		if (bottomRightTileIndex != TileIndex::INVALID_TILE && topLeftTileIndex != TileIndex::INVALID_TILE)
		{
			const float bottomRightTileZ = getTileZ(bottomRightTileIndex);
			const float topLeftTileZ = getTileZ(topLeftTileIndex);
			if (std::abs(z - bottomRightTileZ) > minZDifference
				&& std::abs(z - topLeftTileZ) > minZDifference)
			{
				dy.x = 0.f;
				dy.y = 2.f;
				dy.z = bottomRightTileZ - topLeftTileZ;
			}
		}
		else if (bottomRightTileIndex != TileIndex::INVALID_TILE)
		{
			const float bottomRightTileZ = getTileZ(bottomRightTileIndex);
			if (std::abs(z - bottomRightTileZ) > minZDifference)
			{
				dy.x = 0.f;
				dy.y = 1.f;
				dy.z = bottomRightTileZ - z;
			}
		}
		else if (topLeftTileIndex != TileIndex::INVALID_TILE)
		{
			const float topLeftTileZ = getTileZ(topLeftTileIndex);
			if (std::abs(z - topLeftTileZ) > minZDifference)
			{
				dy.x = 0.f;
				dy.y = 1.f;
				dy.z = z - topLeftTileZ;
			}
		}
	}

	flat::Vector3 normal = flat::normalize(flat::cross(flat::normalize(dx), flat::normalize(dy)));
	getTileSprite(tileIndex).setNormal(normal);
}

void Map::setTileDirty(TileIndex tileIndex)
{
	m_dirtyTiles.insert(tileIndex);
	m_dirtyNormalTiles.insert(tileIndex);
	const flat::Vector2i& tilePosition = getTileXY(tileIndex);

	auto setNeighborNormalTileDirty = [this, &tilePosition](int dx, int dy)
	{
		const TileIndex neighborTileIndex = getTileIndex(tilePosition + flat::Vector2i(dx, dy));
		if (neighborTileIndex != TileIndex::INVALID_TILE)
		{
			m_dirtyNormalTiles.insert(neighborTileIndex);
		}
	};
	setNeighborNormalTileDirty(-1, 0);
	setNeighborNormalTileDirty(0, -1);
	setNeighborNormalTileDirty(0, 1);
	setNeighborNormalTileDirty(1, 0);
}

void Map::updateDirtyTiles()
{
	if (m_neighborTiles.size() < m_tiles.size())
	{
		m_neighborTiles.resize(m_tiles.size());
		for (const std::pair<flat::Vector2i, TileIndex>& pair : m_tilePositionToIndex)
		{
			const flat::Vector2i& tilePosition = pair.first;
			const TileIndex tileIndex = pair.second;
			m_neighborTiles[tileIndex].clear();
			addTileNeighbor(tileIndex, tilePosition + flat::Vector2i(-1, 0));
			addTileNeighbor(tileIndex, tilePosition + flat::Vector2i(0, -1));
			addTileNeighbor(tileIndex, tilePosition + flat::Vector2i(0, 1));
			addTileNeighbor(tileIndex, tilePosition + flat::Vector2i(1, 0));
		}
	}

	for (TileIndex tileIndex : m_dirtyNormalTiles)
	{
		updateTileNormal(tileIndex);
	}
	m_dirtyNormalTiles.clear();
}

void Map::updateAllTiles()
{
	m_dirtyNormalTiles.clear();
	m_neighborTiles.resize(m_tiles.size());
	for (const std::pair<flat::Vector2i, TileIndex>& pair : m_tilePositionToIndex)
	{
		const flat::Vector2i& tilePosition = pair.first;
		const TileIndex tileIndex = pair.second;
		m_neighborTiles[tileIndex].clear();
		addTileNeighbor(tileIndex, tilePosition + flat::Vector2i(-1, 0));
		addTileNeighbor(tileIndex, tilePosition + flat::Vector2i(0, -1));
		addTileNeighbor(tileIndex, tilePosition + flat::Vector2i(0, 1));
		addTileNeighbor(tileIndex, tilePosition + flat::Vector2i(1, 0));
	}

	m_dirtyNormalTiles.clear();
	eachTile([this](TileIndex tileIndex)
	{
		updateTileNormal(tileIndex);
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

void Map::setFogType(fog::Fog::FogType fogType)
{
	m_displayManager->clear();
	m_fog = fog::Fog::createFogFromType(fogType, *this);
	m_fog->init();
}

fog::Fog::FogType Map::getFogType() const
{
	FLAT_ASSERT(m_fog != nullptr);
	return m_fog->getFogType();
}

bool Map::isOnTiles(const flat::AABB2& aabb) const
{
	const int minX = static_cast<int>(aabb.min.x);
	const int minY = static_cast<int>(aabb.min.y);
	const int maxX = static_cast<int>(aabb.max.x);
	const int maxY = static_cast<int>(aabb.max.y);

	for (int x = minX; x <= maxX; ++x)
	{
		for (int y = minY; y <= maxY; ++y)
		{
			if (!isValidTile(getTileIndex(x, y)))
			{
				return false;
			}
		}
	}

	return true;
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

void Map::getAxes(flat::Vector2& xAxis,
				  flat::Vector2& yAxis,
				  flat::Vector2& zAxis) const
{
	 xAxis = m_xAxis;
	 yAxis = m_yAxis;
	 zAxis = m_zAxis;
}

#ifdef FLAT_DEBUG
void Map::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	if (m_enableTileIndicesDebug)
	{
		for (int i = 0, e = static_cast<int>(m_tilePositions.size()); i < e; ++i)
		{
			const TilePosition& tilePosition = m_tilePositions[i];
			flat::Vector3 position;
			position.x = static_cast<float>(tilePosition.xy.x);
			position.y = static_cast<float>(tilePosition.xy.y);
			position.z = m_tileNavigations[i].z;
			std::string str = std::to_string(i);
			debugDisplay.add3dText(position, str);
		}
	}
}
#endif

} // map
} // game



