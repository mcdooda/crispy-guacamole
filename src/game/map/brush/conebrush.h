#ifndef GAME_MAP_BRUSH_CONEBRUSH_H
#define GAME_MAP_BRUSH_CONEBRUSH_H

#include "brush.h"

namespace game
{
namespace map
{
namespace brush
{

class ConeBrush : public Brush
{
	public:
		float getEffect(const flat::Vector2& positionFromCenter) const override;
};

} // brush
} // map
} // game

#endif // GAME_MAP_BRUSH_CONEBRUSH_H



