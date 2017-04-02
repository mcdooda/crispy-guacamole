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
	m_owner->setCanBeSelected(true);
	m_owner->addedToMap.on(this, &PropComponent::addedToMap);
	m_owner->removedFromMap.on(this, &PropComponent::removedFromMap);
}

void PropComponent::update(float currentTime, float elapsedTime)
{
	// nothing to do
}

bool PropComponent::addedToMap(Entity* entity, map::Map* map)
{
	FLAT_ASSERT(entity == m_owner);

	// center the entity on its tile
	flat::Vector3 position = m_owner->getPosition();
	position.x = std::round(position.x);
	position.y = std::round(position.y);
	m_owner->setPosition(position);

	// make it "stand"
	m_owner->setHeading(flat::PI * 0.75f);
	m_owner->setElevation(0.f);

	// occupy the tile
	map::Tile* tile = m_owner->getTile();
	tile->setWalkable(false);
	return true;
}

bool PropComponent::removedFromMap(Entity* entity)
{
	map::Tile* tile = m_owner->getTile();
	tile->setWalkable(true);
	return true;
}

} // prop
} // component
} // entity
} // game



