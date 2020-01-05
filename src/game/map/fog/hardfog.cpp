#include "map/fog/hardfog.h"
#include "map/map.h"
#include "map/displaymanager.h"

namespace game
{
namespace map
{
namespace fog
{

HardFog::HardFog(const map::Map& map) : Fog(map)
{

}

void HardFog::init()
{
	const int tilesCount = m_map.getTilesCount();
	const int propsCount = m_map.getPropsCount();

	m_tileDiscoveryLevels.resize(tilesCount);
	std::fill(m_tileDiscoveryLevels.begin(), m_tileDiscoveryLevels.end(), 0);

	m_propTiles.resize(propsCount);

	map::DisplayManager& displayManager = m_map.getDisplayManager();

	m_observedTiles.resize(tilesCount);
	for (int i = 0; i < tilesCount; ++i)
	{
		const TileIndex tileIndex = static_cast<TileIndex>(i);
		m_observedTiles[tileIndex] = m_map.getTileFromIndex(tileIndex);
		m_observedTiles[tileIndex].getSprite().setColor(flat::video::Color::BLACK);
		displayManager.addTile(tileIndex, &m_observedTiles[tileIndex]);

		const PropIndex propIndex = m_map.getTilePropIndex(tileIndex);
		if (isValidProp(propIndex))
		{
			m_propTiles[propIndex] = tileIndex;
		}
	}


	m_observedProps.resize(propsCount);
	for (int i = 0; i < propsCount; ++i)
	{
		const PropIndex propIndex = static_cast<PropIndex>(i);
		m_observedProps[propIndex] = m_map.getPropFromIndex(propIndex);
		m_observedProps[propIndex].getSprite().setColor(flat::video::Color::BLACK);
		displayManager.addProp(propIndex, &m_observedProps[propIndex]);
	}
}

bool HardFog::needsDiscovering() const
{
	return true;
}

void HardFog::discoverTile(int x, int y, float level)
{
	TileIndex tileIndex = m_map.getTileIndex(flat::Vector2(x, y));
	if (!map::isValidTile(tileIndex))
	{
		return;
	}

	const std::uint8_t newLevel = static_cast<std::uint8_t>(level * 255.f);
	const std::uint8_t discoveryLevel = m_tileDiscoveryLevels[tileIndex];
	if (newLevel <= discoveryLevel)
	{
		return;
	}

	const flat::video::Color newTileColor(level, level, level, 1.f);

	m_tileDiscoveryLevels[tileIndex] = newLevel;
	m_observedTiles[tileIndex] = m_map.getTileFromIndex(tileIndex);
	m_observedTiles[tileIndex].getSprite().setColor(newTileColor);

	PropIndex propIndex = m_map.getTilePropIndex(tileIndex);
	if (isValidProp(propIndex))
	{
		const Prop& prop = m_map.getPropFromIndex(propIndex);
		m_observedProps[propIndex] = prop;
		m_observedProps[propIndex].getSprite().setColor(newTileColor);
	}
}

bool HardFog::isTileDiscovered(TileIndex tileIndex) const
{
	return m_tileDiscoveryLevels[tileIndex] > 0;
}

void HardFog::getTilesFromIndices(const std::vector<TileIndex>& tileIndices, std::vector<const Tile*>& tiles) const
{
	tiles.reserve(tileIndices.size());
	for (TileIndex tileIndex : tileIndices)
	{
		tiles.push_back(&m_observedTiles[tileIndex]);
	}
}

void HardFog::getPropsFromIndices(const std::vector<PropIndex>& propIndices, std::vector<const Prop*>& props) const
{
	props.reserve(propIndices.size());
	for (PropIndex propIndex : propIndices)
	{
		props.push_back(&m_observedProps[propIndex]);
	}
}

void HardFog::addTile(TileIndex tileIndex, const Tile* tile)
{
	m_map.getDisplayManager().addTile(tileIndex, tile);
}

void HardFog::removeTile(TileIndex tileIndex)
{
	m_map.getDisplayManager().removeTile(tileIndex);
}

void HardFog::updateTile(TileIndex tileIndex, const Tile* tile)
{
	if (isTileDiscovered(tileIndex))
	{
		Tile* observedTile = &m_observedTiles[tileIndex];
		*observedTile = *tile;
		m_map.getDisplayManager().updateTile(tileIndex, observedTile);
	}
}

void HardFog::moveTileIndex(TileIndex fromIndex, TileIndex toIndex)
{
	// the previous tile has already been removed at this point
	m_observedTiles[toIndex] = m_observedTiles[fromIndex];
	// we could clean the tile but it does not change anything

	m_tileDiscoveryLevels[toIndex] = m_tileDiscoveryLevels[fromIndex];
	m_tileDiscoveryLevels[fromIndex] = 0;

	m_map.getDisplayManager().moveTileIndex(fromIndex, toIndex);
}

void HardFog::addProp(PropIndex propIndex, const Prop* prop)
{
	m_map.getDisplayManager().addProp(propIndex, prop);
}

void HardFog::removeProp(PropIndex propIndex)
{
	m_map.getDisplayManager().removeProp(propIndex);
}

void HardFog::updateProp(PropIndex propIndex, const Prop* prop)
{
	const TileIndex tileIndex = m_propTiles[propIndex];
	if (isTileDiscovered(tileIndex))
	{
		Prop* observedProp = &m_observedProps[propIndex];
		*observedProp = *prop;
		m_map.getDisplayManager().updateProp(propIndex, observedProp);
	}
}

void HardFog::movePropIndex(PropIndex fromIndex, PropIndex toIndex)
{
	m_observedProps[toIndex] = m_observedProps[fromIndex];
	m_map.getDisplayManager().movePropIndex(fromIndex, toIndex);
}

TileIndex HardFog::getTileIndex(const Tile* tile) const
{
	FLAT_ASSERT(&m_observedTiles[0] <= tile && tile <= &m_observedTiles[m_observedTiles.size() - 1]);
	return static_cast<TileIndex>(tile - &m_observedTiles[0]);
}

TileIndex HardFog::getTileIndex(float x, float y) const
{
	TileIndex tileIndex = m_map.getTileIndex(x, y);
	if (map::isValidTile(tileIndex) && isTileDiscovered(tileIndex))
	{
		return tileIndex;
	}
	return TileIndex::INVALID_TILE;
}

void HardFog::preUpdate()
{
	const int tilesCount = m_map.getTilesCount();
	for (int i = 0; i < tilesCount; ++i)
	{
		const TileIndex tileIndex = static_cast<TileIndex>(i);
		if (isTileDiscovered(tileIndex))
		{
			m_tileDiscoveryLevels[tileIndex] = 0;
			m_observedTiles[tileIndex].getSprite().setColor(flat::video::Color::BLACK);

			const PropIndex propIndex = m_map.getTilePropIndex(tileIndex);
			if (isValidProp(propIndex))
			{
				m_observedProps[propIndex].getSprite().setColor(flat::video::Color::BLACK);
			}
		}
	}
}

void HardFog::postUpdate()
{
	
}

Fog::FogType HardFog::getFogType() const
{
	return FogType::HARD;
}

} // fog
} // map
} // game