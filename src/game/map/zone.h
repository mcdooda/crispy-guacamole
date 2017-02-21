#ifndef GAME_MAP_MAP_ZONE_H
#define GAME_MAP_MAP_ZONE_H

#include <vector>
#include <set>
#include <functional>
#include <flat.h>

namespace game
{
namespace map
{
class Map;
class Tile;

class Zone
{
	public:
		struct Rectangle
		{
			int minX;
			int minY;
			int maxX;
			int maxY;
		};

	public:
		Zone(Map& map);
		~Zone() = default;
		Zone(const Zone& zone) = default;
		Zone& operator=(const Zone& zone) = default;

		const Rectangle& addRectangle(const Rectangle& rectangle);
		void removeRectangle(const Rectangle& rectangle);

		inline const std::vector<Rectangle>& getRectangles() const { return m_rectangles; }

		bool isInside(const flat::Vector2& point) const;

		inline void setColor(const flat::video::Color& color) { m_color = color; }
		inline const flat::video::Color& getColor() const { return m_color; }

		void eachTileIfExists(std::function<void(Tile*)> func);

		const Rectangle* selectRectangle(const flat::Vector2& position) const;

	private:
		void eachRectangleTileIfExists(const Rectangle& rectangle, std::function<void(Tile*)> func);
		static bool isValidRectangle(const Rectangle& rectangle);

	private:
		std::vector<Rectangle> m_rectangles;
		std::set<Tile*> m_tiles;
		flat::video::Color m_color;
		Map& m_map;
};

} // map
} // game


#endif // GAME_MAP_MAP_ZONE_H


