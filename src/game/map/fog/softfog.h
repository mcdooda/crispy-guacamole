#ifndef GAME_MAP_FOG_SOFTFOG_H
#define GAME_MAP_FOG_SOFTFOG_H

#include "map/fog/hardfog.h"

namespace game
{
namespace map
{
class Map;

namespace fog
{

class SoftFog final : public HardFog
{
	public:
		SoftFog(const map::Map& map);

		FogType getFogType() const override;

		bool isTileDiscovered(TileIndex tileIndex) const override;

		void preUpdate() override;

	private:
		static constexpr std::uint8_t TILE_DISCOVERED_LEVEL = 128;
};

} // fog
} // map
} // game

#endif // GAME_MAP_FOG_SOFTFOG_H