#include "movementcomponent.h"
#include "../entity.h"
#include "../entitytemplate.h"

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
		m_owner->setPosition(flat::geometry::Vector3(newPosition2d.x, newPosition2d.y, position.z));
		flat::geometry::Vector2 newMove = pathNextPoint - newPosition2d;
		if (move.dotProduct(newMove) < 0)
		{
			m_path.pop();
			if (!followsPath())
			{
				m_owner->setSpriteColumn(0);
			}
		}
	}
}

bool MovementComponent::followsPath() const
{
	return !m_path.empty();
}

void MovementComponent::addPointOnPath(const flat::geometry::Vector2& point)
{
	m_path.push(point);
}

} // component
} // entity
} // game



