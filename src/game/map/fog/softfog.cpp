#include "map/fog/softfog.h"
#include "map/map.h"

namespace game
{
namespace map
{
namespace fog
{

SoftFog::SoftFog(const map::Map& map) : FogOfWar(map, 128)
{

}

Fog::FogType SoftFog::getFogType() const
{
	return FogType::SOFT;
}

} // fog
} // map
} // game