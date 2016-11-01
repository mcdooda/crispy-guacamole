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

void ProjectileComponent::init()
{
	const float speed = getTemplate()->getSpeed();
	m_speed.x = speed;
	m_speed.y = 0.f;
	m_speed.z = 1.f;

	collision::CollisionComponent* collisionComponent = m_owner->getComponent<collision::CollisionComponent>();
	if (collisionComponent != nullptr)
	{
		collisionComponent->onCollidedWithMap.on([this]() { collided(nullptr); });
		collisionComponent->onCollidedWithEntity.on(this, &ProjectileComponent::collided);
	}
}

void ProjectileComponent::update(float currentTime, float elapsedTime)
{
	const float weight = getTemplate()->getWeight();
	const flat::Vector3 oldSpeed = m_speed;
	m_speed.z -= weight * elapsedTime;
	const flat::Vector3& position = m_owner->getPosition();
	flat::Vector3 newPosition = position + (oldSpeed + m_speed) * 0.5f * elapsedTime;
	m_owner->setPosition(newPosition);
}

void ProjectileComponent::collided(Entity* collidedEntity)
{
	const flat::Vector3& position = m_owner->getPosition();
	const flat::lua::SharedLuaReference<LUA_TFUNCTION>& collidedCallback = getTemplate()->getCollidedCallback();

	lua_State* L = collidedCallback.getLuaState();
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
		collidedCallback.push(L);
		lua::pushEntity(L, m_owner);
		lua::pushEntity(L, collidedEntity);
		lua_call(L, 2, 0);
	}
}

} // projectile
} // component
} // entity
} // game



