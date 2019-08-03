#include "attackcomponent.h"
#include "attackcomponenttemplate.h"
#include "../behavior/behaviorcomponent.h"
#include "../movement/movementcomponent.h"
#include "../../../entity.h"
#include "../../../entityhelper.h"
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
	m_owner->addedToMap.on(this, &AttackComponent::addedToMap);
}

void AttackComponent::deinit()
{
	m_owner->addedToMap.off(this);
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

bool AttackComponent::isInAttackRange(Entity* target) const
{
	const float attackRange = getTemplate()->getAttackRange();

	const float entityRadius = EntityHelper::getRadius(m_owner);
	const float targetRadius = EntityHelper::getRadius(target);

	float distance2 = flat::length2(target->getPosition() - m_owner->getPosition());
	return distance2 <= flat::square(attackRange + entityRadius + targetRadius);
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
	if (m_attackThread.isFinished() && !isBusyForAttacking())
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

	attackStarted();

	const flat::lua::SharedLuaReference<LUA_TFUNCTION>& attack = attackComponentTemplate->getAttack();
	lua_State* L = attack.getLuaState();

	{
		// call attack function
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
		attack.push(L);
		m_attackThread.set(L, -1);
		lua_pop(L, 1);
		m_attackThread.start(m_owner);
	}

	tryEndAttack();
}

void AttackComponent::updateAttack()
{
	// wait for animations and business things to finish before updating the attack thread
	if (isBusyForAttacking())
	{
		return;
	}

	// the thread might be finished but we still update because the entity was busy
	if (m_attackThread.isRunning())
	{
		m_attackThread.update(0);
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

	attackStopped();
}

bool AttackComponent::addedToMap(Entity * entity, map::Map * map)
{
	m_lastAttackTime = 0.f;
	m_attacking = false;
	return true;
}

bool AttackComponent::isBusyForAttacking() const
{
	if (getTemplate()->getAttackDuringMove())
	{
		return m_owner->isBusy(AllComponents & ~movement::MovementComponent::getFlag());
	}

	return m_owner->isBusy();
}

#ifdef FLAT_DEBUG
void AttackComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	if (Entity* target = m_target.getEntity())
	{
		debugDisplay.add3dLine(m_owner->getPosition(), target->getPosition(), flat::video::Color::GREEN);
	}

	debugDisplay.add3dCircle(m_owner->getPosition(), getTemplate()->getAttackRange() + EntityHelper::getRadius(m_owner), flat::video::Color::GREEN, 0.5f);
}
#endif

} // attack
} // component
} // entity
} // game


