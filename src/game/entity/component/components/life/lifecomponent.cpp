#include "lifecomponent.h"

#include "entity/entity.h"
#include "entity/component/components/attack/attackcomponent.h"
#include "entity/component/components/behavior/behaviorcomponent.h"
#include "entity/component/components/movement/movementcomponent.h"

namespace game::entity::component::life
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

	m_damageTakenSlotProxy.init(
		&damageTaken,
		[this](lua_State* L, int amount, Entity* instigator)
		{
			lua_pushinteger(L, amount);
			lua::pushEntity(L, instigator);
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

void LifeComponent::dealDamage(int damage, Entity* instigator)
{
	if (!m_spawning && !m_despawning && m_health > 0 && damage > 0)
	{
		damage = std::min(damage, m_health);
		const int previousHealth = m_health;
		m_health -= damage;
		healthChanged(previousHealth);
		damageTaken(damage, instigator);
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

int LifeComponent::addDamageTakenCallback(lua_State* L, int index)
{
	return m_damageTakenSlotProxy.addCallback(L, index);
}

void LifeComponent::removeDamageTakenCallback(int index)
{
	m_damageTakenSlotProxy.removeCallback(index);
}

int LifeComponent::addDiedCallback(lua_State * L, int index)
{
	return m_diedSlotProxy.addCallback(L, index);
}

void LifeComponent::removeDiedCallback(int index)
{
	m_diedSlotProxy.removeCallback(index);
}

bool LifeComponent::addedToMap(Entity* entity, map::Map* map, EntityUpdater* entityUpdater)
{
	FLAT_ASSERT(entity == m_owner);
	onLive();
	return true;
}

void LifeComponent::onLive()
{
	FLAT_ASSERT(!m_spawning && !m_despawning);

	m_spawning = true;

	disableComponent<attack::AttackComponent>();
	disableComponent<behavior::BehaviorComponent>();
	disableComponent<movement::MovementComponent>();
	
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

	disableComponent<attack::AttackComponent>();
	disableComponent<behavior::BehaviorComponent>();
	disableComponent<movement::MovementComponent>();
	
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
		
		enableComponent<attack::AttackComponent>();
		enableComponent<behavior::BehaviorComponent>();
		enableComponent<movement::MovementComponent>();
	}
}

#ifdef FLAT_DEBUG
void LifeComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	std::string debugString = std::to_string(m_health) + "/" + std::to_string(getMaxHealth()) + "hp";
	if (m_spawning)
	{
		debugString += "\n(spawning)";
	}
	else if (m_despawning)
	{
		debugString += "\n(despawning)";
	}
	debugDisplay.add3dText(m_owner->getPosition(), debugString);
}
#endif

} // game::entity::component::life