#include "attackcomponent.h"
#include "attackcomponenttemplate.h"
#include "../collision/collisioncomponent.h"
#include "../../../entity.h"
#include "../../../lua/entity.h"

namespace game
{
namespace entity
{
namespace component
{
namespace attack
{

void AttackComponent::init()
{
	m_lastAttackTime = 0.f;
}

void AttackComponent::update(float currentTime, float elapsedTime)
{
	const AttackComponentTemplate* attackComponentTemplate = getTemplate();

	const bool autoAttack = attackComponentTemplate->getAutoAttack();
	if (!autoAttack)
	{
		return;
	}

	if (currentTime >= m_lastAttackTime + attackComponentTemplate->getAttackCooldown())
	{
		if (Entity* target = m_target.getEntity())
		{
			const float attackRange = attackComponentTemplate->getAttackRange();

			const collision::CollisionComponentTemplate* collisionComponentTemplate = getTemplate<collision::CollisionComponent>();
			const float radius = collisionComponentTemplate ? collisionComponentTemplate->getRadius() : 0.f;

			const std::shared_ptr<const EntityTemplate>& targetEntityTemplate = target->getEntityTemplate();
			const collision::CollisionComponentTemplate* targetCollisionComponentTemplate = targetEntityTemplate->getComponentTemplate<collision::CollisionComponent>();
			const float targetRadius = targetCollisionComponentTemplate ? targetCollisionComponentTemplate->getRadius() : 0.f;

			float distance2 = flat::length2(target->getPosition() - m_owner->getPosition());
			if (distance2 <= (attackRange + radius + targetRadius) * (attackRange + radius + targetRadius))
			{
				attack(currentTime);
			}
		}
	}
}

void AttackComponent::attack(float currentTime)
{
	m_lastAttackTime = currentTime;

	Entity* target = m_target.getEntity();
	FLAT_ASSERT(target != nullptr);

	const flat::lua::SharedLuaReference<LUA_TFUNCTION>& attackFunc = getTemplate()->getAttackFunc();
	lua_State* L = attackFunc.getLuaState();

	{
		// call attack function
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
		attackFunc.push(L);
		entity::lua::pushEntity(L, m_owner);
		entity::lua::pushEntity(L, target);
		lua_call(L, 2, 0);
	}

}

#ifdef FLAT_DEBUG
void AttackComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	if (Entity* target = m_target.getEntity())
	{
		debugDisplay.addLine(m_owner->getPosition(), target->getPosition(), flat::video::Color::GREEN);
	}
}
#endif

} // attack
} // component
} // entity
} // game


