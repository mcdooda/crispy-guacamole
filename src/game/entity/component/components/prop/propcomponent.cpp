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

	map::Tile* tile = m_owner->getTile();

	// occupy the tile
	const PropComponentTemplate* propComponentTemplate = getTemplate();
	const int width = propComponentTemplate->getWidth();
	const int height = propComponentTemplate->getHeight();
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			map::Tile* tileToOccupy = map->getTileIfWalkable(tile->getX() - i, tile->getY() - j);
			//FLAT_ASSERT(tileToOccupy != nullptr);
			if (tileToOccupy != nullptr)
			{
				tileToOccupy->setWalkable(false);
			}
		}
	}

	// center the entity on its tile
	flat::Vector3 position = m_owner->getPosition();
	position.x = std::round(position.x);
	position.y = std::round(position.y);
	position.z = tile->getZ();
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
	map::Tile* tile = m_owner->getTile();

	const PropComponentTemplate* propComponentTemplate = getTemplate();
	const int width = propComponentTemplate->getWidth();
	const int height = propComponentTemplate->getHeight();
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			map::Tile* tileToOccupy = map->getTileIfExists(tile->getX() - i, tile->getY() - j);
			//FLAT_ASSERT(tileToOccupy != nullptr);
			if (tileToOccupy != nullptr)
			{
				tileToOccupy->setWalkable(true);
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



