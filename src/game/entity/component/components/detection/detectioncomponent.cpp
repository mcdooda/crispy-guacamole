#include <iostream>
#include "detectioncomponent.h"
#include "detectioncomponenttemplate.h"
#include "../../../entity.h"
#include "../../../../map/map.h"

namespace game
{
namespace entity
{
namespace component
{
namespace detection
{

void DetectionComponent::update(float currentTime, float elapsedTime)
{
	const map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map != nullptr);

	const float visionRange = getTemplate()->getVisionRange();
	const float visionRange2 = visionRange * visionRange;

	flat::Vector2 position(m_owner->getPosition());

	// remove entities too far away
	for (std::set<entity::Entity*>::iterator it = m_visibleEntities.begin(); it != m_visibleEntities.end(); )
	{
		entity::Entity* entity = *it;
		flat::Vector2 entityPosition(entity->getPosition());
		if (flat::length2(entityPosition - position) > visionRange2)
		{
			it = m_visibleEntities.erase(it);
			entityLeftVisionRange(entity);
		}
		else
		{
			++it;
		}
	}

	// add entities in vision range
	map->eachEntityInRange(position, visionRange, [this](entity::Entity* entity)
	{
		std::set<entity::Entity*>::iterator it = m_visibleEntities.find(entity);
		if (it == m_visibleEntities.end())
		{
			m_visibleEntities.insert(entity);
			entityEnteredVisionRange(entity);
		}
	});
}

bool DetectionComponent::isVisible(const entity::Entity& target) const
{
	return m_visibleEntities.find(const_cast<entity::Entity*>(&target)) != m_visibleEntities.end();
}

} // detection
} // component
} // entity
} // game


