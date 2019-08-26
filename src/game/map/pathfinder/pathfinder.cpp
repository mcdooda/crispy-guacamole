#include <set>
#include <algorithm>
#include "pathfinder.h"
#include "../map.h"
#include "../tile.h"

namespace game
{
namespace map
{
namespace pathfinder
{

bool Pathfinder::shouldSimplifyPath = true;

Pathfinder::Pathfinder(const Map& map, float jumpHeight, map::Navigability navigabilityMask) :
	m_map(map),
	m_jumpHeight(jumpHeight),
	m_navigabilityMask(navigabilityMask)
{
	
}

bool Pathfinder::findPath(const flat::Vector2& from, const flat::Vector2& to, std::vector<flat::Vector2>& path) const
{
	FLAT_PROFILE("Find path");

	map::TileIndex firstTileIndex = getTileIndexIfNavigable(from.x, from.y, m_navigabilityMask);
	if (firstTileIndex == TileIndex::INVALID_TILE)
	{
		return false;
	}

	map::TileIndex lastTileIndex = getTileIndexIfNavigable(to.x, to.y, m_navigabilityMask);
	if (lastTileIndex == TileIndex::INVALID_TILE)
	{
		return false;
	}
	
	std::set<TileIndex> closedList;
	std::vector<Node> openList;
	std::map<TileIndex, TileIndex> previous;
	
	Node firstNode;
	firstNode.tileIndex = firstTileIndex;
	firstNode.distance = 0.f;
	firstNode.heuristic = 0.f;
	
	openList.push_back(firstNode);
	
	while (!openList.empty())
	{
		Node current = openList.front();
		openList.erase(openList.begin());
		
		TileIndex tileIndex = current.tileIndex;
		//FLAT_DEBUG_ONLY(->setColor(flat::video::Color::BLUE);)
		closedList.insert(tileIndex);
		
		if (tileIndex == lastTileIndex)
		{
			reconstructPath(previous, tileIndex, from, to, path);
			return !path.empty();
		}
		else
		{
			eachNeighborTiles(tileIndex, [&](TileIndex neighborTileIndex)
			{
				if (closedList.count(neighborTileIndex) > 0)
				{
					return;
				}

				Node neighbor;
				neighbor.tileIndex = neighborTileIndex;
				neighbor.distance = current.distance + 1.f;
				const flat::Vector2i& xy = m_map.getTileXY(neighborTileIndex);
				float estimatedDistance = flat::distance(to, flat::Vector2(xy));
				neighbor.heuristic = neighbor.distance + estimatedDistance;

				std::vector<Node>::iterator it = std::find(openList.begin(), openList.end(), neighbor);
				if (it == openList.end())
				{
					// sorted insertion to mimic a priority queue
					std::vector<Node>::iterator it = std::upper_bound(openList.begin(), openList.end(), neighbor);
					openList.insert(it, neighbor);
					previous.emplace(neighborTileIndex, tileIndex);
				}
				else if (neighbor.distance < it->distance)
				{
					it->distance = neighbor.distance;
					it->heuristic = neighbor.heuristic;
					previous.emplace(neighborTileIndex, tileIndex);
				}
			});
		}
	}
	
	return false;
}

TileIndex Pathfinder::getTileIndexIfNavigable(float x, float y, map::Navigability navigabilityMask) const
{
	return m_map.getTileIndexIfNavigable(x, y, navigabilityMask);
}

void Pathfinder::reconstructPath(
	const std::map<TileIndex, TileIndex>& previous,
	TileIndex lastIndex,
	const flat::Vector2& from,
	const flat::Vector2& to,
	std::vector<flat::Vector2>& path) const
{
	path.clear();
	std::map<TileIndex, TileIndex>::const_iterator it;
	map::TileIndex currentIndex = lastIndex;
	while (true)
	{
		it = previous.find(currentIndex);
		if (it != previous.end())
		{
			currentIndex = it->second;
			const flat::Vector2i& xy = m_map.getTileXY(currentIndex);
			path.insert(path.begin(), flat::Vector2(xy));
		}
		else
		{
			break;
		}
	}
	
	if (!path.empty())
	{
		// replace the first tile position by the actual origin
		path[0] = from;
		path.push_back(to);
		
		/*
#ifdef FLAT_DEBUG
			for (const flat::Vector2& p : path)
			{
				const TileIndex tileIndex = m_map.getTileIndex(p.x, p.y);
				const_cast<Map&>(m_map).setTileColor(tileIndex, flat::video::Color::GREEN);
			}
#endif // FLAT_DEBUG
		*/
	
		simplifyPath(path);
		
		/*
#ifdef FLAT_DEBUG
		for (const flat::Vector2& p : path)
		{
			const TileIndex tileIndex = m_map.getTileIndex(p.x, p.y);
			const_cast<Map&>(m_map).setTileColor(tileIndex, flat::video::Color::RED);
		}
#endif // FLAT_DEBUG
		*/
	}
}

void Pathfinder::simplifyPath(std::vector<flat::Vector2>& path) const
{
#ifdef FLAT_DEBUG
	if (!shouldSimplifyPath)
	{
		return;
	}
#endif

	size_t i = path.size() - 1;
	
	while (i >= 2)
	{
		while (i >= 2 && isStraightPath(path[i - 2], path[i]))
		{
			path.erase(path.begin() + i - 1);
			--i;
		}
		--i;
	}
}

bool Pathfinder::isStraightPath(const flat::Vector2& from, const flat::Vector2& to) const
{
	const float delta = 0.4f;
	flat::Vector2 move = to - from;
	flat::Vector2 segment = flat::normalize(move) * delta;
	float numSegments = flat::length(move) / delta;
	TileIndex fromTileIndex = getTileIndexIfNavigable(from.x, from.y, m_navigabilityMask);
	FLAT_ASSERT(fromTileIndex != TileIndex::INVALID_TILE);
	float previousZ = m_map.getTileZ(fromTileIndex);
	for (float f = 1.f; f <= numSegments; ++f)
	{
		flat::Vector2 point = from + segment * f;
		map::TileIndex tileIndex = getTileIndexIfNavigable(point.x, point.y, m_navigabilityMask);
		if (tileIndex == TileIndex::INVALID_TILE || m_map.getTileZ(tileIndex) > previousZ + m_jumpHeight)
			return false;
		
		previousZ = m_map.getTileZ(tileIndex);
	}
	
	return true;
}

void Pathfinder::eachNeighborTiles(TileIndex tileIndex, std::function<void(TileIndex)> func) const
{
	m_map.eachNeighborTilesWithNavigability(tileIndex, m_jumpHeight, m_navigabilityMask, func);
}

} // pathfinder
} // map
} // game



