#ifndef GAME_MAP_NAVIGABILITY_H
#define GAME_MAP_NAVIGABILITY_H

namespace game
{
namespace map
{

enum Navigability : unsigned char
{
	NONE   = 0,
	GROUND = 1 << 1,
	WATER  = 1 << 2,
	ALL    = 0xFF
};

} // map
} // game

#endif // GAME_MAP_NAVIGABILITY_H