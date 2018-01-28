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

void AttackComponent::init(lua_State* L)
{
	m_owner->addedToMap.on(this, &AttackComponent::addedToMap);
}

void AttackComponent::deinit(lua_State* L)
{
	m_owner->addedToMap.off(this);
	m_attackThread.reset(L);
}

void AttackComponent::update(lua_State* L, float currentTime, float elapsedTime)
{
	if (m_attacking)
	{
		updateAttack(L);
	}
	else
	{
		tryBeginAttack(L, currentTime);
	}
}

void AttackComponent::attack(lua_State* L, float currentTime)
{
	beginAttack(L, currentTime);
}

bool AttackComponent::isInAttackRange(Entity* entity) const
{
	const float attackRange = getAttackRange();

	const float targetRadius = EntityHelper::getRadius(entity);

	float distance2 = flat::length2(entity->getPosition() - m_owner->getPosition());
	return distance2 <= (attackRange + targetRadius) * (attackRange + targetRadius);
}

void AttackComponent::tryBeginAttack(lua_State* L, float currentTime)
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
				beginAttack(L, currentTime);
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

void AttackComponent::beginAttack(lua_State* L, float currentTime)
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

	{
		// call attack function
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
		const flat::lua::SharedLuaReference<LUA_TFUNCTION>& attack = attackComponentTemplate->getAttack();
		attack.push(L);
		m_attackThread.set(L, -1);
		lua_pop(L, 1);
		m_attackThread.start(L, m_owner);
	}

	tryEndAttack();
}

void AttackComponent::updateAttack(lua_State* L)
{
	// wait for animations and business things to finish before updating the attack thread
	if (isBusyForAttacking())
	{
		return;
	}

	// the thread might be finished but we still update because the entity was busy
	if (m_attackThread.isRunning())
	{
		m_attackThread.update(L);
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

float AttackComponent::getAttackRange() const
{
	return getTemplate()->getAttackRange() + EntityHelper::getRadius(m_owner);
}

bool AttackComponent::addedToMap(lua_State* L, Entity* entity, map::Map* map)
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

	debugDisplay.add3dCircle(m_owner->getPosition(), getAttackRange(), flat::video::Color::GREEN, 0.5f);
}
#endif

} // attack
} // component
} // entity
} // game


