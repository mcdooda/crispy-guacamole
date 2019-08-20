#ifdef FLAT_DEBUG
#include <sstream>
#endif

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
	m_health = getMaxHealth();
	m_owner->addedToMap.on(this, &LifeComponent::addedToMap);
	m_spawning = false;
	m_despawning = false;
#ifdef FLAT_DEBUG
	m_dead = false;
#endif

	m_healthChangedSlotProxy.init(
		&healthChanged,
		[this](lua_State* L, int previousHealth)
		{
			lua_pushinteger(L, previousHealth);
			lua_pushinteger(L, m_health);
			lua_pushinteger(L, getMaxHealth());
		}
	);

	m_diedSlotProxy.init(
		&die,
		[](lua_State* L) {}
	);
}

void LifeComponent::deinit()
{
	m_owner->addedToMap.off(this);
	healthChanged.off(this);
	m_healthChangedSlotProxy.reset();
	m_diedSlotProxy.reset();
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
		m_spawnDespawnThread.resume(0, 0);
		checkSpawnDespawnThreadFinished();
	}
}

void LifeComponent::kill()
{
	if (!m_spawning && !m_despawning)
	{
		int previousHealth = m_health;
		m_health = 0;
		healthChanged(previousHealth);
		onDie();
	}
}

void LifeComponent::dealDamage(int damage)
{
	if (!m_spawning && !m_despawning && m_health > 0 && damage > 0)
	{
		damage = std::min(damage, m_health);
		const int previousHealth = m_health;
		m_health -= damage;
		healthChanged(previousHealth);
		if (m_health == 0)
		{
			onDie();
		}
	}
}

void LifeComponent::setHealth(int health)
{
	if (m_health != health)
	{
		const int previousHealth = m_health;
		m_health = std::max(0, health);
		healthChanged(previousHealth);
		if (m_health == 0)
		{
			onDie();
		}
	}
}

int LifeComponent::addHealthChangedCallback(lua_State* L, int index)
{
	return m_healthChangedSlotProxy.addCallback(L, index);
}

void LifeComponent::removeHealthChangeCallback(int index)
{
	m_healthChangedSlotProxy.removeCallback(index);
}

int LifeComponent::addDiedCallback(lua_State * L, int index)
{
	return m_diedSlotProxy.addCallback(L, index);
}

void LifeComponent::removeDiedCallback(int index)
{
	m_diedSlotProxy.removeCallback(index);
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
	FLAT_ASSERT(!m_spawning && !m_despawning && !m_dead);
#ifdef FLAT_DEBUG
	m_dead = true;
#endif

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
	std::stringstream debugString;
	debugString << m_health << "/" << getMaxHealth() << "hp";
	if (m_spawning)
	{
		debugString << "\n(spawning)";
	}
	else if (m_despawning)
	{
		debugString << "\n(despawning)";
	}
	debugDisplay.add3dText(m_owner->getPosition(), debugString.str());
}
#endif

} // life
} // component
} // entity
} // game


