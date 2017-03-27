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

bool AttackComponent::isInAttackRange(Entity * entity) const
{
	const float attackRange = getAttackRange();

	const std::shared_ptr<const EntityTemplate>& targetEntityTemplate = entity->getEntityTemplate();
	const collision::CollisionComponentTemplate* targetCollisionComponentTemplate = targetEntityTemplate->getComponentTemplate<collision::CollisionComponent>();
	const float targetRadius = targetCollisionComponentTemplate ? targetCollisionComponentTemplate->getRadius() : 0.f;

	float distance2 = flat::length2(entity->getPosition() - m_owner->getPosition());
	return distance2 <= (attackRange + targetRadius) * (attackRange + targetRadius);
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
			if (isInAttackRange(target))
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
		disableComponent<movement::MovementComponent>();
	}
	
	disableComponent<behavior::BehaviorComponent>();

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
		m_attackThread.update();
	}

	tryEndAttack();
}

void AttackComponent::endAttack()
{
	m_attacking = false;

	const AttackComponentTemplate* attackComponentTemplate = getTemplate();

	if (!attackComponentTemplate->getMoveDuringAttack())
	{
		enableComponent<movement::MovementComponent>();
	}
	
	enableComponent<behavior::BehaviorComponent>();
}

float AttackComponent::getAttackRange() const
{
	const collision::CollisionComponentTemplate* collisionComponentTemplate = getTemplate<collision::CollisionComponent>();
	return getTemplate()->getAttackRange() + (collisionComponentTemplate ? collisionComponentTemplate->getRadius() : 0.f);
}

#ifdef FLAT_DEBUG
void AttackComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	if (Entity* target = m_target.getEntity())
	{
		debugDisplay.addLine(m_owner->getPosition(), target->getPosition(), flat::video::Color::GREEN);
	}

	debugDisplay.addCircle(m_owner->getPosition(), getAttackRange(), flat::video::Color::GREEN, 0.5f);
}
#endif

} // attack
} // component
} // entity
} // game


