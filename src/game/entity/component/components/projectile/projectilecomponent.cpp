#include "projectilecomponent.h"
#include "projectilecomponenttemplate.h"
#include "../collision/collisioncomponent.h"
#include "../sprite/spritecomponent.h"
#include "../../../entity.h"
#include "../../../lua/entity.h"
#include "../../../../map/map.h"

namespace game
{
namespace entity
{
namespace component
{
namespace projectile
{

void ProjectileComponent::init()
{
	m_owner->addedToMap.on(this, &ProjectileComponent::addedToMap);
	m_owner->removedFromMap.on(this, &ProjectileComponent::removedFromMap);
	m_owner->headingChanged.on(this, &ProjectileComponent::headingChanged);

	collision::CollisionComponent* collisionComponent = m_owner->getComponent<collision::CollisionComponent>();
	if (collisionComponent != nullptr)
	{
		collisionComponent->onCollidedWithEntity.on(this, &ProjectileComponent::collidedWithEntity);
		collisionComponent->onCollidedWithMap.on(this, &ProjectileComponent::collidedWithMap);
	}
}

void ProjectileComponent::deinit()
{
	m_owner->addedToMap.off(this);
	m_owner->removedFromMap.off(this);
	m_owner->headingChanged.off(this);

	collision::CollisionComponent* collisionComponent = m_owner->getComponent<collision::CollisionComponent>();
	if (collisionComponent != nullptr)
	{
		collisionComponent->onCollidedWithEntity.off(this);
		collisionComponent->onCollidedWithMap.off(this);
	}
}

void ProjectileComponent::update(float currentTime, float elapsedTime)
{
	const map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map != nullptr);

	const float weight = getTemplate()->getWeight();
	const flat::Vector3 oldSpeed = m_speed;
	m_speed.z -= weight * elapsedTime;
	const flat::Vector3& position = m_owner->getPosition();
	flat::Vector3 newPosition = position + (oldSpeed + m_speed) * 0.5f * elapsedTime;

	const map::Tile* tile = map->getTileIfExists(newPosition.x, newPosition.y);
	if (tile == nullptr)
	{
		m_owner->markForDelete();
	}
	else
	{
		m_owner->setPosition(newPosition);
		const float speedXY = getSpeedXY();
		const float elevation = std::atan2(m_speed.z, speedXY);
		m_owner->setElevation(elevation);
	}
}

void ProjectileComponent::setSpeed(const flat::Vector3& speed)
{
	m_speed = speed;
	const float heading = std::atan2(speed.y, speed.x);
	m_owner->setHeading(heading);
	const float speedXY = getSpeedXY();
	const float elevation = std::atan2(speed.z, speedXY);
	m_owner->setElevation(elevation);
}

bool ProjectileComponent::addedToMap(Entity* entity, map::Map* map)
{
	FLAT_ASSERT(m_owner == entity);

	if (m_owner->hasSprite())
	{
		m_owner->positionChanged.on(this, &ProjectileComponent::updateSpritePosition);

		if (getTemplate()->getRotateSprite())
		{
			m_owner->elevationChanged.on(this, &ProjectileComponent::updateSpriteElevation);
		}

		sprite::SpriteComponent* spriteComponent = m_owner->getComponent<sprite::SpriteComponent>();
		if (spriteComponent != nullptr)
		{
			spriteComponent->playAnimationByName("move", flat::render::AnimatedSprite::INFINITE_LOOP);
		}
	}

	const float projectileSpeed = getTemplate()->getSpeed();

	const float heading = m_owner->getHeading();
	const float elevation = m_owner->getElevation();

	const float speedXY = std::cos(elevation) * projectileSpeed;

	m_speed.x = std::cos(heading) * speedXY;
	m_speed.y = std::sin(heading) * speedXY;
	m_speed.z = std::sin(elevation) * projectileSpeed;

	return true;
}

bool ProjectileComponent::removedFromMap(Entity* entity)
{
	FLAT_ASSERT(m_owner == entity);

	if (m_owner->hasSprite())
	{
		m_owner->positionChanged.off(this);

		if (getTemplate()->getRotateSprite())
		{
			m_owner->elevationChanged.off(this);
		}
	}

	return true;
}

bool ProjectileComponent::headingChanged(float heading)
{
	const float speedXY = getSpeedXY();
	m_speed.x = std::cos(heading) * speedXY;
	m_speed.y = std::sin(heading) * speedXY;

	if (m_owner->hasSprite() && getTemplate()->getRotateSprite())
	{
		updateSpriteRotation();
	}

	return true;
}

bool ProjectileComponent::collided(Entity* collidedEntity, const map::Tile* collidedTile, const flat::Vector3& normal)
{
	if (!isEnabled())	
	{
		return true;
	}

	if (collidedTile == nullptr && collidedEntity == nullptr)
	{
		m_owner->markForDelete();
	}
	else
	{
		getTemplate()->getCollidedCallback().callFunction(
			[this, collidedEntity, &normal](lua_State* L)
		{
			lua::pushEntity(L, m_owner);
			lua::pushEntity(L, collidedEntity);
			flat::lua::pushVector3(L, normal);
		}
		);
	}

	return true;
}

bool ProjectileComponent::collidedWithEntity(Entity* collidedEntity, const flat::Vector3& normal)
{
	return collided(collidedEntity, nullptr, normal);
}

bool ProjectileComponent::collidedWithMap(const map::Tile* tile, const flat::Vector3& normal)
{
	return collided(nullptr, tile, normal);
}

float ProjectileComponent::getSpeedXY() const
{
	return std::sqrt(m_speed.x * m_speed.x + m_speed.y * m_speed.y);
}

bool ProjectileComponent::updateSpritePosition(const flat::Vector3& position)
{
	FLAT_ASSERT(m_owner->hasSprite());

	const map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map != nullptr);

	flat::Vector2 position2d(map->getTransform() * position);
	m_owner->getSprite().setPosition(position2d);
	return true;
}

bool ProjectileComponent::updateSpriteElevation(float elevation)
{
	updateSpriteRotation();
	return true;
}

void ProjectileComponent::updateSpriteRotation() const
{
	FLAT_ASSERT(m_owner->hasSprite() && getTemplate()->getRotateSprite());

	const float heading = m_owner->getHeading();
	const float elevation = m_owner->getElevation();

	const float directionLenXY = std::cos(elevation);
	flat::Vector3 direction(std::cos(heading) * directionLenXY, std::sin(heading) * directionLenXY, std::sin(elevation));

	const map::Map* map = m_owner->getMap();
	FLAT_ASSERT(map != nullptr);

	flat::Vector2 direction2d(map->getTransform() * direction);
	m_owner->getSprite().setRotationZ(flat::vector2_angle(direction2d));
}

#ifdef FLAT_DEBUG
void ProjectileComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	debugDisplay.add3dLine(m_owner->getPosition(), flat::Vector3(m_owner->getPosition().x, m_owner->getPosition().y, 0.f), flat::video::Color::BLACK);
}
#endif

} // projectile
} // component
} // entity
} // game



