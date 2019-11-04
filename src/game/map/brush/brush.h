#ifndef GAME_MAP_BRUSH_BRUSH_H
#define GAME_MAP_BRUSH_BRUSH_H

#include <vector>
#include <flat.h>

#include "../tile.h"

namespace game
{
namespace entity
{
class Entity;
}
namespace map
{
namespace brush
{

struct TileEffect
{
	TileEffect(TileIndex tileIndex, float effect) : tileIndex(tileIndex), effect(effect) {}

	TileIndex tileIndex;
	float effect;
};
using TilesContainer = std::vector<TileEffect>;

struct TileSlotEffect
{
	TileSlotEffect(const flat::Vector2i& position, float effect) : position(position), effect(effect) {}

	flat::Vector2i position;
	float effect;
};
using TileSlotsContainer = std::vector<TileSlotEffect>;

class Brush
{
	public:
		Brush();
		
		inline void setRadius(float radius) { m_radius = radius; }
		inline float getRadius() const { return m_radius; }
		
		inline void setEdgeWidth(float edgeWidth) { m_edgeWidth = edgeWidth; }
		inline float getEdgeWidth() const { return m_edgeWidth; }
		
		virtual float getEffect(const flat::Vector2& positionFromCenter) const = 0;
		
		void getTiles(Map& map, const flat::Vector2& center, TilesContainer& tiles, float minEffect = 0.f) const;
		void getTileSlots(Map& map, const flat::Vector2& center, TileSlotsContainer& tileSlots, float minEffect = 0.f) const;

		float getTileEffect(const Map& map, const flat::Vector2& center, TileIndex tileIndex) const;

		
		void getEntities(Map& map, const flat::Vector2& center, std::vector<entity::Entity*>& entities, float minEffect = 0.f) const;
		
	protected:
		float m_radius;
		float m_edgeWidth;
};

} // brush
} // map
} // game

#endif // GAME_MAP_BRUSH_BRUSH_H



