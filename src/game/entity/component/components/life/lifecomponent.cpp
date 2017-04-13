#include "lifecomponent.h"
#include "../../../entity.h"
#include "../movement/movementcomponent.h"
#include "../behavior/behaviorcomponent.h"

namespace game
{
namespace entity
{
namespace component
{
namespace life
{

void LifeComponent::init()
{
	m_health = getTemplate()->getMaxHealth();
	m_owner->addedToMap.on(this, &LifeComponent::addedToMap);
	m_spawning = false;
	m_despawning = false;
}

void LifeComponent::deinit()
{
	m_owner->addedToMap.off(this);
}

void LifeComponent::update(float currentTime, float elapsedTime)
{
	// wait for animations and business things to finish before updating the attack thread
	if (m_owner->isBusy())
	{
		return;
	}
	
	if (m_spawnDespawnThread.isRunning()) // the thread might be finished but we still update because the entity was busy
	{
		m_spawnDespawnThread.update();
		checkSpawnDespawnThreadFinished();
	}
}

void LifeComponent::kill()
{
	if (!m_spawning && !m_despawning)
	{
		m_health = 0;
		onDie();
	}
}

void LifeComponent::dealDamage(int damage)
{
	if (!m_spawning && !m_despawning)
	{
		damage = std::min(damage, m_health);
		m_health -= damage;
		damageDealt(damage);
		if (m_health == 0)
		{
			onDie();
		}
	}
}

bool LifeComponent::addedToMap(Entity* entity, map::Map* map)
{
	FLAT_ASSERT(entity == m_owner);
	onLive();
	return true;
}

void LifeComponent::onLive()
{
	FLAT_ASSERT(!m_spawning && !m_despawning);

	m_spawning = true;

	disableComponent<movement::MovementComponent>();
	disableComponent<behavior::BehaviorComponent>();
	
	live();
	
	const LifeComponentTemplate* lifeComponentTemplate = getTemplate();
	const flat::lua::SharedLuaReference<LUA_TFUNCTION>& spawnFunc = lifeComponentTemplate->getSpawnFunc();
	if (!spawnFunc.isEmpty())
	{
		lua_State* L = spawnFunc.getLuaState();
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			spawnFunc.push(L);
			m_spawnDespawnThread.set(L, -1);
			lua_pop(L, 1);
			m_spawnDespawnThread.start(m_owner);
		}
	}

	checkSpawnDespawnThreadFinished();
}

void LifeComponent::onDie()
{
	FLAT_ASSERT(!m_spawning && !m_despawning);

	m_despawning = true;

	disableComponent<movement::MovementComponent>();
	disableComponent<behavior::BehaviorComponent>();
	
	const LifeComponentTemplate* lifeComponentTemplate = getTemplate();
	const flat::lua::SharedLuaReference<LUA_TFUNCTION>& despawnFunc = lifeComponentTemplate->getDespawnFunc();
	if (!despawnFunc.isEmpty())
	{
		lua_State* L = despawnFunc.getLuaState();
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			despawnFunc.push(L);
			m_spawnDespawnThread.set(L, -1);
			lua_pop(L, 1);
			m_spawnDespawnThread.start(m_owner);
		}
	}

	checkSpawnDespawnThreadFinished();
}

void LifeComponent::checkSpawnDespawnThreadFinished()
{
	if (m_spawnDespawnThread.isFinished())
	{
		// the entity either finished spawning or despawning
		if (m_despawning)
		{
			m_despawning = false;
			m_owner->markForDelete();
			die();
		}
		else
		{
			m_spawning = false;
		}
		
		enableComponent<movement::MovementComponent>();
		enableComponent<behavior::BehaviorComponent>();
	}
}

#ifdef FLAT_DEBUG
void LifeComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	
}
#endif

} // life
} // component
} // entity
} // game


