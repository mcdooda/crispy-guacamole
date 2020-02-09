#ifndef GAME_MAP_FOG_HARDFOG_H
#define GAME_MAP_FOG_HARDFOG_H

#include "map/fog/fogofwar.h"

namespace game
{
namespace map
{
class Map;

namespace fog
{

class HardFog : public FogOfWar
{
	public:
		HardFog(const map::Map& map);

		FogType getFogType() const override;
};

} // fog
} // map
} // game

#endif // GAME_MAP_FOG_HARDFOG_H