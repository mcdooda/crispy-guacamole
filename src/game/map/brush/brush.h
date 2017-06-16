#ifndef GAME_MAP_BRUSH_BRUSH_H
#define GAME_MAP_BRUSH_BRUSH_H

#include <vector>
#include <flat.h>

namespace game
{
namespace entity
{
class Entity;
}
namespace map
{
class Map;
class Tile;
namespace brush
{

struct TileEffect
{
	TileEffect(Tile* tile, float effect) : tile(tile), effect(effect) {}

	Tile* tile;
	float effect;
};

using TilesContainer = std::vector<TileEffect>;

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
		float getTileEffect(const flat::Vector2& center, const map::Tile* tile) const;
		
		void getEntities(Map& map, const flat::Vector2& center, std::vector<entity::Entity*>& entities, float minEffect = 0.f) const;
		
	protected:
		float m_radius;
		float m_edgeWidth;
};

} // brush
} // map
} // game

#endif // GAME_MAP_BRUSH_BRUSH_H



