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

Pathfinder::Pathfinder(const Map& map, float jumpHeight) :
	m_map(map),
	m_jumpHeight(jumpHeight)
{
	
}

bool Pathfinder::findPath(const flat::Vector2& from, const flat::Vector2& to, std::vector<flat::Vector2>& path) const
{
	const map::Tile* firstTile = getTileIfWalkable(from.x, from.y);
	if (!firstTile)
	{
		return false;
	}
	
	const int toX = static_cast<int>(std::round(to.x));
	const int toY = static_cast<int>(std::round(to.y));
	
	std::set<const map::Tile*> closedList;
	std::vector<Node> openList;
	std::map<const map::Tile*, const map::Tile*> previous;
	
	Node firstNode;
	firstNode.tile = firstTile;
	firstNode.distance = 0.f;
	firstNode.heuristic = 0.f;
	
	openList.push_back(firstNode);
	
	while (!openList.empty())
	{
		Node current = openList.front();
		openList.erase(openList.begin());
		
		const map::Tile* tile = current.tile;
		//FLAT_DEBUG_ONLY(const_cast<map::Tile*>(tile)->setColor(flat::video::Color::BLUE);)
		closedList.insert(tile);
		
		if (tile->getX() == toX && tile->getY() == toY)
		{
			reconstructPath(previous, tile, from, to, path);
			return !path.empty();
		}
		else
		{
			eachNeighborTiles(tile, [&](const map::Tile* neighborTile)
			{
				if (closedList.count(neighborTile) > 0)
				{
					return;
				}

				Node neighbor;
				neighbor.tile = neighborTile;
				neighbor.distance = current.distance + 1.f;
				float estimatedDistance = flat::length(to - flat::Vector2(static_cast<float>(neighborTile->getX()), static_cast<float>(neighborTile->getY())));
				neighbor.heuristic = neighbor.distance + estimatedDistance;

				std::vector<Node>::iterator it = std::find(openList.begin(), openList.end(), neighbor);
				if (it == openList.end())
				{
					// sorted insertion to mimic a priority queue
					std::vector<Node>::iterator it = std::upper_bound(openList.begin(), openList.end(), neighbor);
					openList.insert(it, neighbor);
					previous.emplace(neighborTile, tile);
				}
				else if (neighbor.distance < it->distance)
				{
					it->distance = neighbor.distance;
					it->heuristic = neighbor.heuristic;
					previous.emplace(neighborTile, tile);
				}
			});
		}
	}
	
	return false;
}

const Tile* Pathfinder::getTileIfWalkable(float x, float y) const
{
	return m_map.getTileIfWalkable(x, y);
}

void Pathfinder::reconstructPath(
	const std::map<const map::Tile*, const map::Tile*>& previous,
	const map::Tile* last,
	const flat::Vector2& from,
	const flat::Vector2& to,
	std::vector<flat::Vector2>& path) const
{
	path.clear();
	std::map<const map::Tile*, const map::Tile*>::const_iterator it;
	const map::Tile* current = last;
	while (true)
	{
		it = previous.find(current);
		if (it != previous.end())
		{
			current = it->second;
			path.insert(path.begin(), flat::Vector2(static_cast<float>(current->getX()), static_cast<float>(current->getY())));
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
		
		/*FLAT_DEBUG_ONLY(
			for (const flat::Vector2& p : path)
			{
				const_cast<map::Tile*>(m_map.getTile(p.x, p.y))->setColor(flat::video::Color::GREEN);
			}
		)*/
	
		simplifyPath(path);
		
		/*FLAT_DEBUG_ONLY(
			for (const flat::Vector2& p : path)
			{
				const_cast<map::Tile*>(m_map.getTile(p.x, p.y))->setColor(flat::video::Color::RED);
			}
		)*/
	}
}

void Pathfinder::simplifyPath(std::vector<flat::Vector2>& path) const
{
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
	const map::Tile* fromTile = getTileIfWalkable(from.x, from.y);
	FLAT_ASSERT(fromTile != nullptr);
	float previousZ = fromTile->getZ();
	for (float f = 1.f; f <= numSegments; ++f)
	{
		flat::Vector2 point = from + segment * f;
		const map::Tile* tile = getTileIfWalkable(point.x, point.y);
		if (!tile || tile->getZ() > previousZ + m_jumpHeight)
			return false;
		
		previousZ = tile->getZ();
	}
	
	return true;
}

void Pathfinder::eachNeighborTiles(const Tile* tile, std::function<void(const Tile*)> func) const
{
	tile->eachWalkableNeighborTiles(m_map, m_jumpHeight, func);
}

} // pathfinder
} // map
} // game



