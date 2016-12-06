#include "attackcomponent.h"
#include "attackcomponenttemplate.h"
#include "../behavior/behaviorcomponent.h"
#include "../collision/collisioncomponent.h"
#include "../movement/movementcomponent.h"
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
	m_attacking = false;
}

void AttackComponent::update(float currentTime, float elapsedTime)
{
	if (m_attacking)
	{
		updateAttack();
	}
	else
	{
		tryBeginAttack(currentTime);
	}
}

void AttackComponent::attack(float currentTime)
{
	beginAttack(currentTime);
}

void AttackComponent::tryBeginAttack(float currentTime)
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
				beginAttack(currentTime);
			}
		}
	}
}

void AttackComponent::tryEndAttack()
{
	if (m_attackThread.isFinished() && !m_owner->isBusy())
	{
		endAttack();
	}
}

void AttackComponent::beginAttack(float currentTime)
{
	m_lastAttackTime = currentTime;
	m_attacking = true;

	Entity* target = m_target.getEntity();
	FLAT_ASSERT(target != nullptr);

	const AttackComponentTemplate* attackComponentTemplate = getTemplate();

	if (!attackComponentTemplate->getMoveDuringAttack())
	{
		movement::MovementComponent* movementComponent = m_owner->getComponent<movement::MovementComponent>();
		if (movementComponent)
		{
			movementComponent->disable();
		}
	}

	behavior::BehaviorComponent* behaviorComponent = m_owner->getComponent<behavior::BehaviorComponent>();
	if (behaviorComponent)
	{
		behaviorComponent->disable();
	}

	const flat::lua::SharedLuaReference<LUA_TFUNCTION>& attackFunc = attackComponentTemplate->getAttackFunc();
	lua_State* L = attackFunc.getLuaState();

	{
		// call attack function
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
		attackFunc.push(L);
		m_attackThread.set(L, -1);
		lua_pop(L, 1);
		m_attackThread.start(m_owner);
	}

	tryEndAttack();
}

void AttackComponent::updateAttack()
{
	// wait for animations and business things to finish before updating the attack thread
	if (m_owner->isBusy())
	{
		return;
	}

	// the thread might be finished but we still update because the entity was busy
	if (m_attackThread.isRunning())
	{
		m_attackThread.update(m_owner);
	}

	tryEndAttack();
}

void AttackComponent::endAttack()
{
	m_attacking = false;

	const AttackComponentTemplate* attackComponentTemplate = getTemplate();

	if (!attackComponentTemplate->getMoveDuringAttack())
	{
		movement::MovementComponent* movementComponent = m_owner->getComponent<movement::MovementComponent>();
		if (movementComponent)
		{
			FLAT_ASSERT(!movementComponent->isEnabled());
			movementComponent->enable();
		}
	}

	behavior::BehaviorComponent* behaviorComponent = m_owner->getComponent<behavior::BehaviorComponent>();
	if (behaviorComponent)
	{
		FLAT_ASSERT(!behaviorComponent->isEnabled());
		behaviorComponent->enable();
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


