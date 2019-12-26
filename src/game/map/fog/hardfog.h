#ifndef GAME_MAP_FOG_HARDFOG_H
#define GAME_MAP_FOG_HARDFOG_H

#include "map/fog/fog.h"

namespace game
{
namespace map
{
class Map;

namespace fog
{

class HardFog final : public Fog
{
	public:
		HardFog(const map::Map& map);

		FogType getFogType() const override;

		void init() override;

		bool needsDiscovering() const override;
		void discoverTile(int x, int y, float level) override;
		bool isTileDiscovered(TileIndex tileIndex) const override;

		void getTilesFromIndices(const std::vector<TileIndex>& tileIndices, std::vector<const Tile*>& tiles) const override;
		void getPropsFromIndices(const std::vector<PropIndex>& propIndices, std::vector<const Prop*>& props) const override;

		void removeTile(TileIndex tileIndex) override;
		void updateTile(TileIndex tileIndex, const Tile* tile) override;
		void moveTileIndex(TileIndex fromIndex, TileIndex toIndex) override;

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
};

} // fog
} // map
} // game

#endif // GAME_MAP_FOG_HARDFOG_H