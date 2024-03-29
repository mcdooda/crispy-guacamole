#include "behaviorcomponent.h"
#include "behaviorcomponenttemplate.h"
#include "behaviorevent.h"

#include "entity/entity.h"
#include "entity/entitytemplate.h"
#include "entity/entityhelper.h"
#include "entity/component/componenttype.h"

#include "entity/component/components/detection/detectioncomponent.h"
#include "entity/component/components/life/lifecomponent.h"

namespace game
{
namespace entity
{
namespace component
{
namespace behavior
{

void BehaviorComponent::init()
{
	m_behaviorRuntime.setEntity(m_owner);
	m_owner->addedToMap.on(this, &BehaviorComponent::addedToMap);

	detection::DetectionComponent* detectionComponent = m_owner->getComponent<detection::DetectionComponent>();
	if (detectionComponent != nullptr)
	{
		if (m_behaviorRuntime.isEventHandled<EntityEnteredVisionRangeBehaviorEvent>())
		{
			detectionComponent->entityEnteredVisionRange.on(this, &BehaviorComponent::entityEnteredVisionRange);
		}
		if (m_behaviorRuntime.isEventHandled<EntityLeftVisionRangeBehaviorEvent>())
		{
			detectionComponent->entityLeftVisionRange.on(this, &BehaviorComponent::entityLeftVisionRange);
		}
	}

	life::LifeComponent* lifeComponent = m_owner->getComponent<life::LifeComponent>();
	if (lifeComponent != nullptr)
	{
		if (m_behaviorRuntime.isEventHandled<DamageTakenBehaviorEvent>())
		{
			lifeComponent->damageTaken.on(this, &BehaviorComponent::damageTaken);
		}
	}
}

void BehaviorComponent::deinit()
{
	//m_behaviorRuntime.setEntity(nullptr); // TODO: uncomment this
	m_owner->addedToMap.off(this);

	if (detection::DetectionComponent* detectionComponent = m_owner->getComponent<detection::DetectionComponent>())
	{
		if (m_behaviorRuntime.isEventHandled<EntityEnteredVisionRangeBehaviorEvent>())
		{
			detectionComponent->entityEnteredVisionRange.off(this);
		}
		if (m_behaviorRuntime.isEventHandled<EntityLeftVisionRangeBehaviorEvent>())
		{
			detectionComponent->entityLeftVisionRange.off(this);
		}
	}

	life::LifeComponent* lifeComponent = m_owner->getComponent<life::LifeComponent>();
	if (lifeComponent != nullptr)
	{
		if (m_behaviorRuntime.isEventHandled<DamageTakenBehaviorEvent>())
		{
			lifeComponent->damageTaken.off(this);
		}
	}
}

void BehaviorComponent::update(float time, float dt)
{
	tryInteracting();

	const int maxTicks = 2;
	for (int numTicks = 0; !m_owner->isBusy() && numTicks < maxTicks; ++numTicks)
	{
		m_behaviorRuntime.update(time);
	}
	
	/*
#ifdef FLAT_DEBUG
	const char* entityName = m_owner->getEntityTemplate()->getName().c_str();
	const char* stateName = m_behaviorRuntime.getCurrentStateName().c_str();
	std::string file;
	int line;
	getThreadDebugInfo(file, line);
	FLAT_ASSERT_MSG(m_owner->isBusy(), "Behavior warning: %s still idling after %d behavior ticks in state %s\nCurrently in file %s, line %d", entityName, maxTicks, stateName, file.c_str(), line);
#endif
	*/
}

void BehaviorComponent::cancelCurrentAction()
{
	cancelInteraction();
}

void BehaviorComponent::enterState(const char* stateName)
{
	m_behaviorRuntime.enterState(stateName);
}

bool BehaviorComponent::hasState(const char* stateName) const
{
	return m_behaviorRuntime.hasState(stateName);
}

bool BehaviorComponent::setInteractionIfCompatible(const char* stateName, entity::Entity* interactionEntity)
{
	if (canInteract(stateName))
	{
		m_interactionStateName = stateName;
		m_interactionEntity = interactionEntity->getHandle();
		return true;
	}
	return false;
}

bool BehaviorComponent::canInteract(const char* stateName) const
{
	return m_behaviorRuntime.hasState(stateName);
}

void BehaviorComponent::sleep(float time, float duration)
{
	m_behaviorRuntime.sleep(time, duration);
}

bool BehaviorComponent::handlePlayerMoveOrder(const flat::Vector2& point, Entity* interactionEntity)
{
	if (m_behaviorRuntime.isEventHandled<PlayerMoveOrderEvent>())
	{
		return m_behaviorRuntime.handleEvent<PlayerMoveOrderEvent>(point, interactionEntity);
	}
	return true;
}

bool BehaviorComponent::addedToMap(Entity* entity, map::Map* map, EntityUpdater* entityUpdater)
{
	FLAT_ASSERT(entity == m_owner);
	if (isEnabled())
	{
		enterState("init");
	}
	return true;
}

bool BehaviorComponent::entityEnteredVisionRange(Entity* entity)
{
	m_behaviorRuntime.handleEvent<EntityEnteredVisionRangeBehaviorEvent>(entity);
	return true;
}

bool BehaviorComponent::entityLeftVisionRange(Entity* entity)
{
	m_behaviorRuntime.handleEvent<EntityLeftVisionRangeBehaviorEvent>(entity);
	return true;
}

bool BehaviorComponent::damageTaken(int amount, Entity* instigator)
{
	m_behaviorRuntime.handleEvent<DamageTakenBehaviorEvent>(amount, instigator);
	return true;
}

void BehaviorComponent::tryInteracting()
{
	Entity* interactionEntity = m_interactionEntity.getEntity();
	if (interactionEntity != nullptr)
	{
		FLAT_ASSERT(!m_interactionStateName.empty() && m_behaviorRuntime.hasState(m_interactionStateName.c_str()));
		if (EntityHelper::getDistanceBetweenEntitiesWithRadius(m_owner, interactionEntity) < 0.01f || !m_owner->acceptsMoveOrders())
		{
			m_owner->cancelCurrentActions(AllComponents & ~behavior::BehaviorComponent::getFlag());

			const std::string originalInteractionStateName = m_interactionStateName;
			enterState(originalInteractionStateName.c_str());

			// if the behavior did not change the interaction entity, cancel the interaction
			if (m_interactionEntity == interactionEntity && m_interactionStateName == originalInteractionStateName)
			{
				cancelInteraction();
			}
		}
	}
	else if (!m_interactionStateName.empty())
	{
		enterState("missingInteractionEntity");
		if (!m_interactionEntity.isValid())
		{
			cancelInteraction();
		}
	}
}

void BehaviorComponent::cancelInteraction()
{
	m_interactionEntity = nullptr;
	m_interactionStateName.clear();
}

#ifdef FLAT_DEBUG
void BehaviorComponent::getThreadDebugInfo(std::string& file, int& line) const
{
	file.clear();
	line = 0;

	const flat::lua::UniqueLuaReference<LUA_TTHREAD>& luaThreadRef = m_behaviorRuntime.getThread().getThread();
	if (!luaThreadRef.isEmpty())
	{
		lua_State* L = luaThreadRef.getLuaState();
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			luaThreadRef.push(L);
			lua_State* L1 = lua_tothread(L, -1);

			lua_Debug ar;
			lua_getstack(L1, 1, &ar);
			lua_getinfo(L1, "Sl", &ar);
			line = ar.currentline;
			file = ar.short_src;

			lua_pop(L, 1);
		}
	}
}

void BehaviorComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	int currentLine;
	std::string currentFile;
	getThreadDebugInfo(currentFile, currentLine);

	const std::string currentStateName = m_behaviorRuntime.getCurrentStateName().empty() ? "<none>" : m_behaviorRuntime.getCurrentStateName();
	if (currentLine != 0)
	{
		if (m_behaviorRuntime.getPreviousStateName().empty())
		{
			debugDisplay.add3dText(
				m_owner->getPosition(),
				currentStateName
					+ "\n" + currentFile + ":" + std::to_string(currentLine),
				flat::video::Color::BLUE
			);
		}
		else
		{
			debugDisplay.add3dText(
				m_owner->getPosition(),
				currentStateName
					+ "\npreviously " + m_behaviorRuntime.getPreviousStateName()
					+ "\n" + currentFile + ":" + std::to_string(currentLine),
				flat::video::Color::BLUE
			);
		}
	}
	else
	{
		if (m_behaviorRuntime.getPreviousStateName().empty())
		{
			debugDisplay.add3dText(
				m_owner->getPosition(),
				m_behaviorRuntime.getCurrentStateName() + " / previous: " + m_behaviorRuntime.getPreviousStateName(),
				flat::video::Color::RED
			);
		}
		else
		{
			debugDisplay.add3dText(
				m_owner->getPosition(),
				currentStateName
					+ "\npreviously " + m_behaviorRuntime.getPreviousStateName(),
				flat::video::Color::RED
			);
		}
	}

	Entity* interactionEntity = m_interactionEntity.getEntity();
	if (interactionEntity != nullptr || !m_interactionStateName.empty())
	{
		std::string str;
		if (interactionEntity != nullptr)
		{
			debugDisplay.add3dLine(
				m_owner->getPosition(),
				interactionEntity->getPosition(),
				flat::video::Color::BLACK
			);
			str += std::string("Distance: ") + std::to_string(EntityHelper::getDistanceBetweenEntitiesWithRadius(m_owner, interactionEntity));
		}
		if (!m_interactionStateName.empty())
		{
			str += std::string("\n") + m_interactionStateName;
		}
		debugDisplay.add3dText(m_owner->getPosition() + flat::Vector3(0.f, 0.f, 1.f), str);
	}
}
#endif

} // behavior
} // component
} // entity
} // game



