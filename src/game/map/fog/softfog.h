#ifndef GAME_MAP_FOG_SOFTFOG_H
#define GAME_MAP_FOG_SOFTFOG_H

#include "map/fog/fogofwar.h"

namespace game
{
namespace map
{
class Map;

namespace fog
{

class SoftFog final : public FogOfWar
{
	public:
		SoftFog(const map::Map& map);

		FogType getFogType() const override;
};

} // fog
} // map
} // game

#endif // GAME_MAP_FOG_SOFTFOG_H