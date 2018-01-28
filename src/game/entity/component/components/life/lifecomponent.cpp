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

void LifeComponent::init(lua_State* L)
{
	m_health = getMaxHealth();
	m_owner->addedToMap.on(this, &LifeComponent::addedToMap);
	m_spawning = false;
	m_despawning = false;
}

void LifeComponent::deinit(lua_State* L)
{
	m_owner->addedToMap.off(this);

	for (flat::lua::UniqueLuaReference<LUA_TFUNCTION>& healthChangedRef : m_healthChangedRefs)
	{
		healthChangedRef.reset(L);
	}
	m_healthChangedRefs.clear();

	m_spawnDespawnThread.reset(L);
}

void LifeComponent::update(lua_State* L, float currentTime, float elapsedTime)
{
	// wait for animations and business things to finish before updating the attack thread
	if (m_owner->isBusy())
	{
		return;
	}
	
	if (m_spawnDespawnThread.isRunning()) // the thread might be finished but we still update because the entity was busy
	{
		m_spawnDespawnThread.update(L);
		checkSpawnDespawnThreadFinished();
	}
}

void LifeComponent::kill(lua_State* L)
{
	if (!m_spawning && !m_despawning)
	{
		int previousHealth = m_health;
		m_health = 0;
		healthChanged(previousHealth);
		onDie(L);
	}
}

void LifeComponent::dealDamage(lua_State* L, int damage)
{
	if (!m_spawning && !m_despawning)
	{
		damage = std::min(damage, m_health);
		int previousHealth = m_health;
		m_health -= damage;
		healthChanged(previousHealth);
		if (m_health == 0)
		{
			onDie(L);
		}
	}
}

int LifeComponent::addHealthChangedCallback(lua_State* L, int index)
{
	int callbackIndex = static_cast<int>(m_healthChangedRefs.size());
	m_healthChangedRefs.emplace_back();
	m_healthChangedRefs.back().set(L, index);
	return callbackIndex;
}

void LifeComponent::pushHealthChangedCallback(lua_State* L, int callbackIndex) const
{
	m_healthChangedRefs[callbackIndex].push(L);
}

bool LifeComponent::addedToMap(lua_State* L, Entity* entity, map::Map* map)
{
	FLAT_ASSERT(entity == m_owner);
	onLive(L);
	return true;
}

void LifeComponent::onLive(lua_State* L)
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
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
		spawnFunc.push(L);
		m_spawnDespawnThread.set(L, -1);
		lua_pop(L, 1);
		m_spawnDespawnThread.start(L, m_owner);
	}

	checkSpawnDespawnThreadFinished();
}

void LifeComponent::onDie(lua_State* L)
{
	FLAT_ASSERT(!m_spawning && !m_despawning);

	m_despawning = true;

	disableComponent<movement::MovementComponent>();
	disableComponent<behavior::BehaviorComponent>();
	
	const LifeComponentTemplate* lifeComponentTemplate = getTemplate();
	const flat::lua::SharedLuaReference<LUA_TFUNCTION>& despawnFunc = lifeComponentTemplate->getDespawnFunc();
	if (!despawnFunc.isEmpty())
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
		despawnFunc.push(L);
		m_spawnDespawnThread.set(L, -1);
		lua_pop(L, 1);
		m_spawnDespawnThread.start(L, m_owner);
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


