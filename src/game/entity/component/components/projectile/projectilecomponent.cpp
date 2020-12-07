#include "projectilecomponent.h"
#include "projectilecomponenttemplate.h"

#include "entity/entity.h"
#include "entity/lua/entity.h"
#include "entity/component/components/collision/collisioncomponent.h"
#include "entity/component/components/collision/collisionbox.h"
#include "entity/component/components/sprite/spritecomponent.h"

#include "map/map.h"

namespace game::entity::component::projectile
{

void ProjectileComponent::init()
{
	m_weight = getTemplate()->getWeight();

	m_owner->addedToMap.on(this, &ProjectileComponent::addedToMap);
	m_owner->removedFromMap.on(this, &ProjectileComponent::removedFromMap);
	m_owner->headingChanged.on(this, &ProjectileComponent::headingChanged);
	m_owner->elevationChanged.on(this, &ProjectileComponent::elevationChanged);

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
	m_owner->elevationChanged.off(this);

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

	const float speed = flat::length(m_speed);

	flat::Vector3 newSpeed = m_speed;

	// compute speed depending on weight
	const flat::Vector3& position = m_owner->getPosition();
	flat::Vector3 newPosition;
	if (m_weight == 0.f)
	{
		const flat::Vector3 forward = m_owner->getForward();
		newSpeed = forward * speed;
	}
	else
	{
		newSpeed.z -= m_weight * elapsedTime;
	}

	// compute speed to follow the target
	Entity* target = m_target.getEntity();
	if (target != nullptr)
	{
		const flat::Vector3 direction = flat::normalize(target->getCenter() - m_owner->getPosition());
		const flat::Vector2 direction2d = flat::normalize(flat::Vector2(direction));

		const float newSpeedXY = std::sqrt(flat::square(newSpeed.x) + flat::square(newSpeed.y));
		newSpeed.x = direction2d.x * newSpeedXY;
		newSpeed.y = direction2d.y * newSpeedXY;

		if (flat::dot(direction, flat::normalize(m_speed)) > flat::dot(direction, flat::normalize(newSpeed)))
		{
			newSpeed = direction * getTemplate()->getSpeed();
		}
	}

	newPosition = position + (m_speed + newSpeed) * 0.5f * elapsedTime;

	const map::TileIndex tileIndex = map->getTileIndex(newPosition.x, newPosition.y);
	if (tileIndex == map::TileIndex::INVALID_TILE)
	{
		m_owner->markForDelete();
	}
	else
	{
		const float tileZ = map->getTileZ(tileIndex);
		newPosition.z = std::max(newPosition.z, tileZ - Entity::MIN_Z_EPSILON);
		m_owner->setPosition(newPosition);
		setSpeed(newSpeed);
		checkTargetCollision();
	}
}

void ProjectileComponent::setSpeed(const flat::Vector3& speed)
{
	if (speed != m_speed)
	{
		m_speed = speed;
		const float heading = std::atan2(speed.y, speed.x);
		m_owner->setHeading(heading);
		FLAT_ASSERT(flat::distance(m_speed, speed) < 0.1f);

		const float speedXY = getSpeedXY();
		const float elevation = std::atan2(speed.z, speedXY);
		m_owner->setElevation(elevation);
		FLAT_ASSERT(flat::distance(m_speed, speed) < 0.1f);
	}
}

bool ProjectileComponent::addedToMap(Entity* entity, map::Map* map, EntityUpdater* entityUpdater)
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
			spriteComponent->playAnimationByName("move", flat::render::AnimatedSprite::INFINITE_LOOP, true);
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
	if (m_owner->hasSprite() && getTemplate()->getRotateSprite())
	{
		updateSpriteRotation();
	}

	return true;
}

bool ProjectileComponent::elevationChanged(float elevation)
{
	if (m_owner->hasSprite() && getTemplate()->getRotateSprite())
	{
		updateSpriteRotation();
	}

	return true;
}

void ProjectileComponent::checkTargetCollision()
{
	Entity* target = m_target.getEntity();
	if (target == nullptr)
	{
		return;
	}

	collision::CollisionComponent* collisionComponent = m_owner->getComponent<collision::CollisionComponent>();
	if (collisionComponent == nullptr || collisionComponent->getTemplate()->shouldSeparateFromOtherEntities())
	{
		return;
	}

	collision::CollisionComponent* targetCollisionComponent = target->getComponent<collision::CollisionComponent>();
	if (targetCollisionComponent == nullptr)
	{
		return;
	}

	flat::Vector3 penetration;
	if (collision::CollisionBox::collides(m_owner->getPosition(), target->getPosition(), collisionComponent->getTemplate()->getCollisionBox(), targetCollisionComponent->getTemplate()->getCollisionBox(), penetration))
	{
		collidedWithEntity(target, flat::normalize(penetration));
	}
}

bool ProjectileComponent::collided(Entity* collidedEntity, map::TileIndex collidedTileIndex, const flat::Vector3& normal)
{
	if (!isEnabled())	
	{
		return true;
	}

	if (collidedTileIndex == map::TileIndex::INVALID_TILE && collidedEntity == nullptr)
	{
		m_owner->markForDelete();
	}
	else
	{
		getTemplate()->getCollidedCallback().call(m_owner, collidedEntity, normal);
	}

	return true;
}

bool ProjectileComponent::collidedWithEntity(Entity* collidedEntity, const flat::Vector3& normal)
{
	sprite::SpriteComponent* spriteComponent = collidedEntity->getComponent<sprite::SpriteComponent>();
	if (spriteComponent != nullptr && m_owner->hasSprite())
	{
		spriteComponent->attachSprite(static_cast<flat::render::Sprite&>(m_owner->getSprite()));
	}

	return collided(collidedEntity, map::TileIndex::INVALID_TILE, normal);
}

bool ProjectileComponent::collidedWithMap(map::TileIndex tileIndex, const flat::Vector3& normal)
{
	return collided(nullptr, tileIndex, normal);
}

float ProjectileComponent::getSpeedXY() const
{
	return std::sqrt(flat::square(m_speed.x) + flat::square(m_speed.y));
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

} // game::entity::component::projectile