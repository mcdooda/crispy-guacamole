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

void LifeComponent::update(float currentTime, float elapsedTime)
{
	// wait for animations and business things to finish before updating the attack thread
	if (m_owner->isBusy())
	{
		return;
	}
	
	if (m_spawnDespawnThread.isRunning()) // the thread might be finished but we still update because the entity was busy
	{
		m_spawnDespawnThread.update(m_owner);
		checkSpawnDespawnThreadFinished();
	}
}

void LifeComponent::kill()
{
	m_health = 0;
	onDespawn();
}

void LifeComponent::dealDamage(int damage)
{
	damage = std::min(damage, m_health);
	m_health -= damage;
	damageDealt(damage);
	if (m_health == 0)
	{
		onDespawn();
	}
}

void LifeComponent::addedToMap(map::Map* map)
{
	onSpawn();
}

void LifeComponent::onSpawn()
{
	disableComponent<movement::MovementComponent>();
	disableComponent<behavior::BehaviorComponent>();
	
	spawn();
	
	const LifeComponentTemplate* lifeComponentTemplate = getTemplate();
	const flat::lua::SharedLuaReference<LUA_TFUNCTION>& spawnFunc = lifeComponentTemplate->getSpawnFunc();
	if (spawnFunc)
	{
		m_spawning = true;
		lua_State* L = spawnFunc.getLuaState();
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			spawnFunc.push(L);
			m_spawnDespawnThread.set(L, -1);
			lua_pop(L, 1);
			m_spawnDespawnThread.start(m_owner);
			
			checkSpawnDespawnThreadFinished();
		}
	}
}

void LifeComponent::onDespawn()
{
	disableComponent<movement::MovementComponent>();
	disableComponent<behavior::BehaviorComponent>();
	
	const LifeComponentTemplate* lifeComponentTemplate = getTemplate();
	const flat::lua::SharedLuaReference<LUA_TFUNCTION>& despawnFunc = lifeComponentTemplate->getDespawnFunc();
	if (despawnFunc)
	{
		m_despawning = true;
		lua_State* L = despawnFunc.getLuaState();
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			despawnFunc.push(L);
			m_spawnDespawnThread.set(L, -1);
			lua_pop(L, 1);
			m_spawnDespawnThread.start(m_owner);
			
			checkSpawnDespawnThreadFinished();
		}
	}
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
			despawn();
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


