#ifndef GAME_MAP_FOG_FOG_H
#define GAME_MAP_FOG_FOG_H

#include <flat.h>

#include "map/tile.h"

namespace game
{
namespace map
{
class Map;

namespace fog
{

class Fog
{
	public:
		enum FogType : std::uint8_t
		{
			NONE,
			HARD,
			SOFT
		};

	public:
		Fog(const map::Map& map);

		static std::unique_ptr<Fog> createFogFromType(FogType fogType, const map::Map& map);

		virtual FogType getFogType() const = 0;

		virtual void init();

		virtual bool needsDiscovering() const = 0;
		void discover(const flat::Vector2& xy, float radius);
		virtual void discoverTile(int x, int y, float level) = 0;
		virtual bool isTileDiscovered(TileIndex tileIndex) const = 0;

		virtual void addTile(TileIndex tileIndex, const Tile* tile) = 0;
		virtual void removeTile(TileIndex tileIndex) = 0;
		virtual void updateTile(TileIndex tileIndex, const Tile* tile) = 0;
		virtual void moveTileIndex(TileIndex fromIndex, TileIndex toIndex) = 0;

		virtual void addProp(PropIndex propIndex, const Prop* prop) = 0;
		virtual void removeProp(PropIndex propIndex) = 0;
		virtual void updateProp(PropIndex propIndex, const Prop* prop) = 0;
		virtual void movePropIndex(PropIndex fromIndex, PropIndex toIndex) = 0;

		virtual void getTilesFromIndices(const std::vector<TileIndex>& tileIndices, std::vector<const Tile*>& tiles) const = 0;
		virtual void getPropsFromIndices(const std::vector<PropIndex>& propIndices, std::vector<const Prop*>& props) const = 0;

		virtual TileIndex getTileIndex(const Tile* tile) const = 0;
		virtual TileIndex getTileIndex(float x, float y) const = 0;

		virtual void preUpdate() = 0;
		virtual void postUpdate() = 0;

	protected:
		const map::Map& m_map;
};

} // fog
} // map
} // game

#endif // GAME_MAP_FOG_FOG_H