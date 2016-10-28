#ifndef GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIORRUNTIME_H
#define GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIORRUNTIME_H

#include <flat.h>
#include <memory>

namespace game
{
namespace entity
{
class Entity;
namespace component
{
namespace behavior
{
class Behavior;

class BehaviorRuntime final
{
	public:
		BehaviorRuntime();
		BehaviorRuntime(const BehaviorRuntime&) = delete;
		void operator=(const BehaviorRuntime&) = delete;
		~BehaviorRuntime();
		
		inline void setEntity(Entity* entity) { m_entity = entity; }
		
		void enterState(const char* stateName);
		void updateCurrentState();
		void update();

		FLAT_DEBUG_ONLY(inline const std::string& getCurrentStateName() const { return m_currentStateName; })
		
	private:
		const Behavior& getBehavior() const;
		
	private:
		Entity* m_entity;
		int m_coroutineRef;
		FLAT_DEBUG_ONLY(std::string m_currentStateName;)
};

} // behavior
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIORRUNTIME_H


