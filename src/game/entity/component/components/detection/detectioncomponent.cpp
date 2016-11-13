#include <iostream>
#include "detectioncomponent.h"
#include "detectioncomponenttemplate.h"
#include "../../../entity.h"
#include "../../../../map/map.h"
#include "../../../../game.h"

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
	for (std::set<Entity*>::iterator it = m_visibleEntities.begin(); it != m_visibleEntities.end(); )
	{
		Entity* entity = *it;
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
	map->eachEntityInRange(position, visionRange, [this](Entity* entity)
	{
		if (entity == m_owner)
		{
			return;
		}

		std::set<Entity*>::iterator it = m_visibleEntities.find(entity);
		if (it == m_visibleEntities.end())
		{
			m_visibleEntities.insert(entity);
			entityEnteredVisionRange(entity);
		}
	});
}

bool DetectionComponent::isVisible(const Entity& target) const
{
	return m_visibleEntities.find(const_cast<Entity*>(&target)) != m_visibleEntities.end();
}

#ifdef FLAT_DEBUG
void DetectionComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	flat::Vector3 ownerPosition = m_owner->getPosition() + flat::Vector3(0.f, 0.f, 0.5f);
	for (Entity* entity : m_visibleEntities)
	{
		debugDisplay.addLine(ownerPosition, entity->getPosition(), flat::video::Color::RED, flat::video::Color::BLUE, 2.5f);
	}
}
#endif

} // detection
} // component
} // entity
} // game


