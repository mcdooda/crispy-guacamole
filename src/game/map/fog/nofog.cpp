#include "map/fog/nofog.h"
#include "map/map.h"
#include "map/displaymanager.h"

namespace game
{
namespace map
{
namespace fog
{

NoFog::NoFog(const map::Map& map) : Fog(map)
{

}

void NoFog::init()
{
	m_map.addAllTilesToDisplayManager();
	m_map.addAllPropsToDisplayManager();
}

bool NoFog::needsDiscovering() const
{
	return false;
}

void NoFog::discoverTile(int x, int y, float level)
{
	FLAT_ASSERT_MSG(false, "Should never be called");
}

bool NoFog::isTileDiscovered(TileIndex tileIndex) const
{
	return true;
}

void NoFog::getTilesFromIndices(const std::vector<TileIndex>& tileIndices, std::vector<const Tile*>& tiles) const
{
	m_map.getTilesFromIndices(tileIndices, tiles);
}

void NoFog::getPropsFromIndices(const std::vector<PropIndex>& propIndices, std::vector<const Prop*>& props) const
{
	m_map.getPropsFromIndices(propIndices, props);
}

void NoFog::addTile(TileIndex tileIndex, const Tile* tile)
{
	m_map.getDisplayManager().addTile(tileIndex, tile);
}

void NoFog::removeTile(TileIndex tileIndex)
{
	m_map.getDisplayManager().removeTile(tileIndex);
}

void NoFog::updateTile(TileIndex tileIndex, const Tile* tile)
{
	m_map.getDisplayManager().updateTile(tileIndex, tile);
}

void NoFog::moveTileIndex(TileIndex fromIndex, TileIndex toIndex)
{
	m_map.getDisplayManager().moveTileIndex(fromIndex, toIndex);
}

void NoFog::addProp(PropIndex propIndex, const Prop* prop)
{
	m_map.getDisplayManager().addProp(propIndex, prop);
}

void NoFog::removeProp(PropIndex propIndex)
{
	m_map.getDisplayManager().removeProp(propIndex);
}

void NoFog::updateProp(PropIndex propIndex, const Prop* prop)
{
	m_map.getDisplayManager().updateProp(propIndex, prop);
}

void NoFog::movePropIndex(PropIndex fromIndex, PropIndex toIndex)
{
	m_map.getDisplayManager().movePropIndex(fromIndex, toIndex);
}

map::TileIndex NoFog::getTileIndex(const Tile* tile) const
{
	return m_map.getTileIndex(tile);
}

map::TileIndex NoFog::getTileIndex(float x, float y) const
{
	return m_map.getTileIndex(x, y);
}

void NoFog::preUpdate()
{

}

void NoFog::postUpdate()
{

}

Fog::FogType NoFog::getFogType() const
{
	return FogType::NONE;
}

} // fog
} // map
} // game