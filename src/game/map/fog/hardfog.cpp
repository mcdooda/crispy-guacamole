#include "map/fog/hardfog.h"
#include "map/map.h"
#include "map/displaymanager.h"

namespace game
{
namespace map
{
namespace fog
{

HardFog::HardFog(const map::Map& map) : FogOfWar(map, 0)
{

}

Fog::FogType HardFog::getFogType() const
{
	return FogType::HARD;
}

} // fog
} // map
} // game