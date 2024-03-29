#ifndef GAME_MAP_FOG_FOGOFWAR_H
#define GAME_MAP_FOG_FOGOFWAR_H

#include "map/fog/fog.h"

namespace game
{
namespace map
{
class Map;

namespace fog
{

class FogOfWar : public Fog
{
	public:
		FogOfWar(const map::Map& map, std::uint8_t tileDiscoveredLevel);

		void init() override;

		bool needsDiscovering() const override;
		void discoverTile(int x, int y, float level) override;
		bool isTileDiscovered(TileIndex tileIndex) const override;

		void getTilesFromIndices(const std::vector<TileIndex>& tileIndices, std::vector<const Tile*>& tiles) const override;
		void getPropsFromIndices(const std::vector<PropIndex>& propIndices, std::vector<const Prop*>& props) const override;

		void addTile(TileIndex tileIndex, const Tile* tile) override;
		void removeTile(TileIndex tileIndex) override;
		void updateTile(TileIndex tileIndex, const Tile* tile) override;
		void moveTileIndex(TileIndex fromIndex, TileIndex toIndex) override;

		void addProp(PropIndex propIndex, const Prop* prop) override;
		void removeProp(PropIndex propIndex) override;
		void updateProp(PropIndex propIndex, const Prop* prop) override;
		void movePropIndex(PropIndex fromIndex, PropIndex toIndex) override;

		TileIndex getTileIndex(const Tile* tile) const override;
		TileIndex getTileIndex(float x, float y) const override;

		void preUpdate() override;
		void postUpdate() override;

private:
		std::vector<std::uint8_t> m_tileDiscoveryLevels;
		std::vector<Tile> m_observedTiles;
		std::vector<Prop> m_observedProps;
		std::vector<TileIndex> m_propTiles;
		std::set<TileIndex> m_frameObservedTiles;
		std::uint8_t m_tileDiscoveredLevel;
};

} // fog
} // map
} // game

#endif // GAME_MAP_FOG_FOGOFWAR_H