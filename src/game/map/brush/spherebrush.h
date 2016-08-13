#ifndef GAME_MAP_SPHEREBRUSH_H
#define GAME_MAP_SPHEREBRUSH_H

#include "brush.h"

namespace game
{
namespace map
{
namespace brush
{

class SphereBrush : public Brush
{
	public:
		float getEffect(const flat::geometry::Vector2& positionFromCenter) const override;
};

} // brush
} // map
} // game

#endif // GAME_MAP_SPHEREBRUSH_H



