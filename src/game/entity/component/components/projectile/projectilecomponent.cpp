#include "projectilecomponent.h"
#include "projectilecomponenttemplate.h"
#include "../collision/collisioncomponent.h"
#include "../../../entity.h"
#include "../../../lua/entity.h"

namespace game
{
namespace entity
{
namespace component
{
namespace projectile
{

void ProjectileComponent::init(lua_State* L)
{
	m_owner->addedToMap.on(this, &ProjectileComponent::addedToMap);
	m_owner->headingChanged.on(this, &ProjectileComponent::headingChanged);

	collision::CollisionComponent* collisionComponent = m_owner->getComponent<collision::CollisionComponent>();
	if (collisionComponent != nullptr)
	{
		collisionComponent->onCollidedWithMap.on(this, &ProjectileComponent::collidedWithMap);
		collisionComponent->onCollidedWithEntity.on(this, &ProjectileComponent::collided);
	}
}

void ProjectileComponent::deinit(lua_State* L)
{
	m_owner->addedToMap.off(this);
	m_owner->headingChanged.off(this);

	collision::CollisionComponent* collisionComponent = m_owner->getComponent<collision::CollisionComponent>();
	if (collisionComponent != nullptr)
	{
		collisionComponent->onCollidedWithMap.off(this);
		collisionComponent->onCollidedWithEntity.off(this);
	}
}

void ProjectileComponent::update(lua_State* L, float currentTime, float elapsedTime)
{
	const float weight = getTemplate()->getWeight();
	const flat::Vector3 oldSpeed = m_speed;
	m_speed.z -= weight * elapsedTime;
	const flat::Vector3& position = m_owner->getPosition();
	flat::Vector3 newPosition = position + (oldSpeed + m_speed) * 0.5f * elapsedTime;
	m_owner->setPosition(newPosition);
	const float speedXY = getSpeedXY();
	float elevation = std::atan2(m_speed.z, speedXY);
	m_owner->setElevation(elevation);
}

bool ProjectileComponent::addedToMap(lua_State* L, Entity* entity, map::Map* map)
{
	const float projectileSpeed = getTemplate()->getSpeed();

	const float heading = m_owner->getHeading();
	const float elevation = m_owner->getElevation();

	const float speedXY = std::cos(elevation) * projectileSpeed;

	m_speed.x = std::cos(heading) * speedXY;
	m_speed.y = std::sin(heading) * speedXY;
	m_speed.z = std::sin(elevation) * projectileSpeed;

	return true;
}

bool ProjectileComponent::headingChanged(float heading)
{
	const float speedXY = getSpeedXY();
	m_speed.x = std::cos(heading) * speedXY;
	m_speed.y = std::sin(heading) * speedXY;
	return true;
}

bool ProjectileComponent::collided(lua_State* L, Entity* collidedEntity)
{
	if (!isEnabled())
		return true;

	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
		const flat::lua::SharedLuaReference<LUA_TFUNCTION>& collidedCallback = getTemplate()->getCollidedCallback();
		collidedCallback.push(L);
		lua::pushEntity(L, m_owner);
		lua::pushEntity(L, collidedEntity);
		lua_call(L, 2, 1);
		bool keepEnabled = lua_toboolean(L, -1) == 1;
		if (!keepEnabled)
		{
			incDisableLevel();
		}
		lua_pop(L, 1);
	}

	return true;
}

bool ProjectileComponent::collidedWithMap(lua_State* L)
{
	return collided(L, nullptr);
}

float ProjectileComponent::getSpeedXY() const
{
	return std::sqrt(m_speed.x * m_speed.x + m_speed.y * m_speed.y);
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



