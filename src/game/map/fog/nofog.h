#ifndef GAME_MAP_FOG_NOFOG_H
#define GAME_MAP_FOG_NOFOG_H

#include "map/fog/fog.h"

namespace game
{
namespace map
{
class Map;

namespace fog
{

class NoFog final : public Fog
{
	public:
		NoFog(const map::Map& map);

		FogType getFogType() const override;

		void init() override;

		bool needsDiscovering() const override;
		void discoverTile(int x, int y, float level) override;
		bool isTileDiscovered(TileIndex tileIndex) const override;

		void addTile(TileIndex tileIndex, const Tile* tile) override;
		void removeTile(TileIndex tileIndex) override;
		void updateTile(TileIndex tileIndex, const Tile* tile) override;
		void moveTileIndex(TileIndex fromIndex, TileIndex toIndex) override;

		void addProp(PropIndex propIndex, const Prop* prop) override;
		void removeProp(PropIndex propIndex) override;
		void updateProp(PropIndex propIndex, const Prop* prop) override;
		void movePropIndex(PropIndex fromIndex, PropIndex toIndex) override;

		void getTilesFromIndices(const std::vector<TileIndex>& tileIndices, std::vector<const Tile*>& tiles) const override;
		void getPropsFromIndices(const std::vector<PropIndex>& propIndices, std::vector<const Prop*>& props) const override;

		TileIndex getTileIndex(const Tile* tile) const override;
		TileIndex getTileIndex(float x, float y) const override;

		void preUpdate() override;
		void postUpdate() override;
};

} // fog
} // map
} // game

#endif // GAME_MAP_FOG_NOFOG_H