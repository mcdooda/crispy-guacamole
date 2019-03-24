#include "propcomponent.h"
#include "propcomponenttemplate.h"
#include "../../../entity.h"
#include "../../../../map/map.h"
#include "../../../../map/tile.h"

namespace game
{
namespace entity
{
namespace component
{
namespace prop
{

void PropComponent::init()
{
	m_owner->addedToMap.on(this, &PropComponent::addedToMap);
	m_owner->removedFromMap.on(this, &PropComponent::removedFromMap);
}

void PropComponent::deinit()
{
	m_owner->addedToMap.off(this);
	m_owner->removedFromMap.off(this);
}

bool PropComponent::addedToMap(Entity* entity, map::Map* map)
{
	FLAT_ASSERT(entity == m_owner);

	map::TileIndex tileIndex = m_owner->getTileIndexFromPosition();
	FLAT_ASSERT(tileIndex != map::TileIndex::INVALID);
	const flat::Vector2i& tilePosition = map->getTileXY(tileIndex);

	// occupy the tile
	const PropComponentTemplate* propComponentTemplate = getTemplate();
	const int width = propComponentTemplate->getWidth();
	const int height = propComponentTemplate->getHeight();
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			map::TileIndex tileToOccupyIndex = map->getTileIndexIfNavigable(tilePosition.x - i, tilePosition.y - j, map::Navigability::ALL);
			//FLAT_ASSERT(tileToOccupy != nullptr);
			if (tileToOccupyIndex != map::TileIndex::INVALID)
			{
				map->setTileNavigability(tileToOccupyIndex, map::Navigability::NONE);
			}
		}
	}

	// center the entity on its tile
	flat::Vector3 position = m_owner->getPosition();
	position.x = std::round(position.x);
	position.y = std::round(position.y);
	position.z = map->getTileZ(tileIndex);
	m_owner->setPosition(position);

	// set aabb
	flat::AABB3 aabb;
	aabb.min = position - flat::Vector3(width - 0.5f, height - 0.5f, 0.f);
	aabb.max = position + flat::Vector3(0.5f, 0.5f, 5.f);
	m_owner->setWorldSpaceAABB(aabb);
	m_owner->setAABBCanChange(false);

	//  update sprite
	if (m_owner->hasSprite())
	{
		flat::render::BaseSprite& sprite = m_owner->getSprite();
		flat::Vector2 position2d(map->getTransform() * position);
		sprite.setPosition(position2d);
	}

	return true;
}

bool PropComponent::removedFromMap(Entity* entity)
{
	map::Map* map = m_owner->getMap();
	map::TileIndex tileIndex = m_owner->getTileIndexFromPosition();
	FLAT_ASSERT(tileIndex != map::TileIndex::INVALID);
	const flat::Vector2i& tilePosition = map->getTileXY(tileIndex);

	const PropComponentTemplate* propComponentTemplate = getTemplate();
	const int width = propComponentTemplate->getWidth();
	const int height = propComponentTemplate->getHeight();
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			map::TileIndex tileToOccupyIndex = map->getTileIndex(tilePosition.x - i, tilePosition.y - j);
			//FLAT_ASSERT(tileToOccupy != nullptr);
			if (tileToOccupyIndex != map::TileIndex::INVALID)
			{
				map->setTileNavigability(tileToOccupyIndex, map::Navigability::ALL);
			}
		}
	}
	return true;
}

#ifdef FLAT_DEBUG
void PropComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	debugDisplay.add3dAABB(m_owner->getWorldSpaceAABB());
}
#endif

} // prop
} // component
} // entity
} // game



