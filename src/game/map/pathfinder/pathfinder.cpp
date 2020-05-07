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


Pathfinder::Pathfinder(const Map& map, float jumpHeight, map::Navigability navigabilityMask) :
	m_map(map),
	m_jumpHeight(jumpHeight),
	m_navigabilityMask(navigabilityMask)
{

}

Result Pathfinder::findPath(const Request& request, Path& path) const
{
	std::vector<flat::Vector2> points;
	Result result = findPath(request.from, request.to, request.allowPartialResult, points);
	if (result != Result::FAILURE)
	{
		path.m_positions = points;
	}
	return result;
}

Result Pathfinder::findPath(const flat::Vector2& from, const flat::Vector2& to, bool allowPartialResults, std::vector<flat::Vector2>& path) const
{
	FLAT_PROFILE("Find path");

	path.clear();

	const map::TileIndex firstTileIndex = getTileIndexIfNavigable(from.x, from.y, m_navigabilityMask);
	if (!isValidTile(firstTileIndex))
	{
		return Result::FAILURE;
	}

	const map::TileIndex lastTileIndex = getTileIndexIfNavigable(to.x, to.y, m_navigabilityMask);
	if (!isValidTile(lastTileIndex))
	{
		return Result::FAILURE;
	}

	if (firstTileIndex == lastTileIndex)
	{
		path.push_back(from);
		path.push_back(to);
		return Result::SUCCESS;
	}

	// TODO check if a straight path exists before A*

	std::set<TileIndex> closedList;
	std::vector<Node> openList;
	std::map<TileIndex, TileIndex> previous;

	Node firstNode;
	firstNode.tileIndex = firstTileIndex;
	firstNode.distance = 0.f;
	firstNode.heuristic = 0.f;

	openList.push_back(firstNode);

	int currentIteration = 0;

	while (!openList.empty())
	{
		Node current = openList.front();
		openList.erase(openList.begin());

		TileIndex tileIndex = current.tileIndex;
		//FLAT_DEBUG_ONLY(->setColor(flat::video::Color::BLUE);)
		closedList.insert(tileIndex);

		if (tileIndex == lastTileIndex)
		{
			reconstructPath(previous, tileIndex, from, to, Result::SUCCESS, path);
			FLAT_ASSERT(!path.empty());
			return Result::SUCCESS;
		}

		if (allowPartialResults)
		{
			++currentIteration;
			if (currentIteration >= DEFAULT_ITERATION_LIMIT)
			{
				reconstructPath(previous, tileIndex, from, to, Result::PARTIAL, path);
				FLAT_ASSERT(!path.empty());
				return Result::PARTIAL;
			}
		}

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

	return Result::FAILURE;
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
	Result pathfindingResult,
	std::vector<flat::Vector2>& path) const
{
	FLAT_ASSERT(pathfindingResult != Result::FAILURE);

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
		if (pathfindingResult == Result::SUCCESS)
		{
			path.push_back(to);
		}
	}
}

void Pathfinder::eachNeighborTiles(TileIndex tileIndex, std::function<void(TileIndex)> func) const
{
	m_map.eachNeighborTilesWithNavigability(tileIndex, m_jumpHeight, m_navigabilityMask, func);
}

} // pathfinder
} // map
} // game



