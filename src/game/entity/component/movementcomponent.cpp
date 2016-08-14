#include <set>
#include <algorithm>
#include "movementcomponent.h"
#include "../entity.h"
#include "../entitytemplate.h"
#include "../../map/map.h"
#include "../../map/tile.h"

namespace game
{
namespace entity
{
namespace component
{

void MovementComponent::update(float currentTime, float elapsedTime)
{
	if (!followsPath())
		return;
	
	flat::geometry::Vector2& pathNextPoint = m_path.front();
	const flat::geometry::Vector3& position = m_owner->getPosition();
	flat::geometry::Vector2 position2d = flat::geometry::Vector2(position.x, position.y);
	flat::geometry::Vector2 move = pathNextPoint - position2d;
	if (move.lengthSquared() > 0.f)
	{
		m_owner->setHeading(move.angle());
		flat::geometry::Vector2 direction = move.normalize();
		const EntityTemplate* entityTemplatePtr = m_owner->getEntityTemplate().get();
		float speed = entityTemplatePtr->getSpeed();
		flat::geometry::Vector2 newPosition2d = position2d + direction * speed * elapsedTime;
		flat::geometry::Vector2 newMove = pathNextPoint - newPosition2d;
		if (move.dotProduct(newMove) <= 0.f)
		{
			newPosition2d = pathNextPoint;
			m_path.pop();
			if (!followsPath())
			{
				m_owner->movementStopped();
			}
		}
		m_owner->setPosition(flat::geometry::Vector3(newPosition2d.x, newPosition2d.y, position.z));
	}
	
	separateFromAdjacentTiles();
}

bool MovementComponent::isBusy() const
{
	return followsPath();
}

bool MovementComponent::followsPath() const
{
	return !m_path.empty();
}

void MovementComponent::addPointOnPath(const flat::geometry::Vector2& point)
{
	bool startMovement = false;
	flat::geometry::Vector2 startingPoint;
	if (m_path.empty())
	{
		const flat::geometry::Vector3& position = m_owner->getPosition();
		startingPoint.x = position.x;
		startingPoint.y = position.y;
		startMovement = true;
	}
	else
	{
		startingPoint = m_path.back();
	}
	
	const float minDistanceBetweenPoints = 0.3f;
	if ((point - startingPoint).lengthSquared() > minDistanceBetweenPoints * minDistanceBetweenPoints)
	{
		std::vector<flat::geometry::Vector2> path;
		if (findPath(startingPoint, point, path))
		{
			path.erase(path.begin());
			for (const flat::geometry::Vector2& point : path)
			{
				m_path.push(point);
			}
		}
		else
		{
			m_path.push(point);
		}
		
		if (startMovement)
		{
			m_owner->movementStarted();
		}
	}
}

void MovementComponent::separateFromAdjacentTiles()
{
	const map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map);
	const map::Tile* tile = m_owner->getTile();
	FLAT_ASSERT(tile);
	const int tileX = static_cast<float>(tile->getX());
	const int tileY = static_cast<float>(tile->getY());
	
	const flat::geometry::Vector3& position = m_owner->getPosition();
	const float z = position.z;
	FLAT_ASSERT(z >= tile->getZ());
	
	flat::geometry::Vector2 newPosition2d(position.x, position.y);
	
	const EntityTemplate* entityTemplatePtr = m_owner->getEntityTemplate().get();
	const float radius = entityTemplatePtr->getRadius();
	
	// directly adjacent tiles
	// top right
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX - 1, tileY);
		if (!tile2 || tile2->getZ() > z)
		{
			newPosition2d.x = std::max(newPosition2d.x, static_cast<float>(tileX) - 0.5f + radius);
		}
	}
	// bottom left
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX + 1, tileY);
		if (!tile2 || tile2->getZ() > z)
		{
			newPosition2d.x = std::min(newPosition2d.x, static_cast<float>(tileX) + 0.5f - radius);
		}
	}
	// top left
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX, tileY - 1);
		if (!tile2 || tile2->getZ() > z)
		{
			newPosition2d.y = std::max(newPosition2d.y, static_cast<float>(tileY) - 0.5f + radius);
		}
	}
	// bottom right
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX, tileY + 1);
		if (!tile2 || tile2->getZ() > z)
		{
			newPosition2d.y = std::min(newPosition2d.y, static_cast<float>(tileY) + 0.5f - radius);
		}
	}
	
	// diagonals
	// top
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX - 1, tileY - 1);
		if (!tile2 || tile2->getZ() > z)
		{
			flat::geometry::Vector2 corner(static_cast<float>(tileX) - 0.5f, static_cast<float>(tileY) - 0.5f);
			flat::geometry::Vector2 toCorner = corner - newPosition2d;
			if (toCorner.lengthSquared() < radius * radius)
			{
				newPosition2d = corner - toCorner.normalize() * radius;
			}
		}
	}
	// bottom
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX + 1, tileY + 1);
		if (!tile2 || tile2->getZ() > z)
		{
			flat::geometry::Vector2 corner(static_cast<float>(tileX) + 0.5f, static_cast<float>(tileY) + 0.5f);
			flat::geometry::Vector2 toCorner = corner - newPosition2d;
			if (toCorner.lengthSquared() < radius * radius)
			{
				newPosition2d = corner - toCorner.normalize() * radius;
			}
		}
	}
	// left
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX + 1, tileY - 1);
		if (!tile2 || tile2->getZ() > z)
		{
			flat::geometry::Vector2 corner(static_cast<float>(tileX) + 0.5f, static_cast<float>(tileY) - 0.5f);
			flat::geometry::Vector2 toCorner = corner - newPosition2d;
			if (toCorner.lengthSquared() < radius * radius)
			{
				newPosition2d = corner - toCorner.normalize() * radius;
			}
		}
	}
	// right
	{
		const map::Tile* tile2 = map->getTileIfWalkable(tileX - 1, tileY + 1);
		if (!tile2 || tile2->getZ() > z)
		{
			flat::geometry::Vector2 corner(static_cast<float>(tileX) - 0.5f, static_cast<float>(tileY) + 0.5f);
			flat::geometry::Vector2 toCorner = corner - newPosition2d;
			if (toCorner.lengthSquared() < radius * radius)
			{
				newPosition2d = corner - toCorner.normalize() * radius;
			}
		}
	}
	
	m_owner->setPosition(flat::geometry::Vector3(newPosition2d.x, newPosition2d.y, z));
}

bool MovementComponent::findPath(const flat::geometry::Vector2& from, const flat::geometry::Vector2& to, std::vector<flat::geometry::Vector2>& path) const
{
	const map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map);
	
	const int fromX = round(from.x);
	const int fromY = round(from.y);
	const map::Tile* firstTile = map->getTileIfWalkable(fromX, fromY);
	if (!firstTile)
	{
		return false;
	}
	
	const int toX = round(to.x);
	const int toY = round(to.y);
	
	std::set<const map::Tile*> closedList;
	std::vector<Node> openList;
	std::map<const map::Tile*, const map::Tile*> previous;
	
	Node firstNode;
	firstNode.tile = firstTile;
	firstNode.distance = 0.f;
	firstNode.heuristic = 0.f;
	
	openList.push_back(firstNode);
	
	std::vector<const map::Tile*> neighborTiles;
	
	while (!openList.empty())
	{
		Node current = openList.front();
		openList.erase(openList.begin());
		
		const map::Tile* tile = current.tile;
		closedList.insert(tile);
		
		FLAT_DEBUG_ONLY(const_cast<map::Tile*>(tile)->setColor(flat::video::Color::BLUE);)
		if (tile->getX() == toX && tile->getY() == toY)
		{
			reconstructPath(previous, tile, from, to, path);
			return true;
		}
		else
		{
			tile->getWalkableNeighborTiles(*map, neighborTiles);
			for (const map::Tile* neighborTile : neighborTiles)
			{
				// already in closedList
				{
					if (closedList.count(neighborTile) > 0)
					{
						continue;
					}
				}
				
				Node neighbor;
				neighbor.tile = neighborTile;
				neighbor.distance = current.distance + 1.f;
				float estimatedDistance = (to - flat::geometry::Vector2(neighborTile->getX(), neighborTile->getY())).length();
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
			}
		}
	}
	
	return false;
}

void MovementComponent::reconstructPath(
	const std::map<const map::Tile*, const map::Tile*> previous,
	const map::Tile* last,
	const flat::geometry::Vector2& from,
	const flat::geometry::Vector2& to,
	std::vector<flat::geometry::Vector2>& path) const
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
			path.insert(path.begin(), flat::geometry::Vector2(current->getX(), current->getY()));
		}
		else
		{
			break;
		}
	}
	
	// replace the 
	path[0] = from;
	path.push_back(to);
	
	simplifyPath(path);
	
	FLAT_DEBUG_ONLY(
		const map::Map* map = m_owner->getMap();
		for (const flat::geometry::Vector2& p : path)
		{
			const map::Tile* tile = map->getTile(round(p.x), round(p.y));
			const_cast<map::Tile*>(tile)->setColor(flat::video::Color::RED);
		}
	)
}

void MovementComponent::simplifyPath(std::vector<flat::geometry::Vector2>& path) const
{
	unsigned int i = path.size() - 1;
	while (i >= 2)
	{
		while (i >= 2 && isStraightPath(path[i], path[i - 2]))
		{
			path.erase(path.begin() + i - 1);
			--i;
		}
		--i;
	}
}

bool MovementComponent::isStraightPath(const flat::geometry::Vector2& from, const flat::geometry::Vector2& to) const
{
	if (from.x == to.x || from.y == to.y)
		return true;
	
	const float delta = 0.4f;
	const map::Map* map = m_owner->getMap();
	flat::geometry::Vector2 move = to - from;
	flat::geometry::Vector2 segment = move.normalize() * delta;
	float numSegments = move.length() / delta;
	for (float f = 1.f; f <= numSegments; ++f)
	{
		flat::geometry::Vector2 point = from + segment * f;
		if (!map->getTileIfWalkable(point.getRoundX(), point.getRoundY()))
			return false;
	}
	
	
	return true;
}

} // component
} // entity
} // game



