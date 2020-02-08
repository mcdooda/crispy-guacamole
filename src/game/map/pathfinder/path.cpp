#include "path.h"
#include "map/map.h"

namespace game::map::pathfinder
{

#ifdef FLAT_DEBUG
bool Path::shouldSimplifyPath = true;
#endif

void Path::simplify(const Map& map, float jumpHeight, Navigability navigabilityMask)
{
#ifdef FLAT_DEBUG
	if (!shouldSimplifyPath)
		return;
	/*for (const auto& position : m_positions)
	{
		const_cast<Map&>(map).setTileColor(map.getTileIndex(position), flat::video::Color::GREEN);
	}*/
#endif // FLAT_DEBUG
	int i = static_cast<int>(m_positions.size()) - 1;

	while (i >= 2)
	{
		while (i >= 2 && map.straightPathExists(m_positions[i - 2], m_positions[i], jumpHeight, navigabilityMask))
		{
			m_positions.erase(m_positions.begin() + i - 1);
			--i;
		}
		--i;
	}
#ifdef FLAT_DEBUG
	/*for (const auto& position : m_positions)
	{
		const_cast<Map&>(map).setTileColor(map.getTileIndex(position), flat::video::Color::RED);
	}*/
#endif // FLAT_DEBUG
}

void Path::insertPath(const Path& path)
{
	m_positions.insert(m_positions.end(), path.m_positions.begin() + 1, path.m_positions.end());
}

void Path::insertPoint(const flat::Vector2& point)
{
	m_positions.push_back(point);
}

} // game::map::pathfinder