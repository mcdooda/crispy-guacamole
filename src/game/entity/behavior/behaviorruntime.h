#ifndef GAME_ENTITY_BEHAVIOR_BEHAVIORRUNTIME_H
#define GAME_ENTITY_BEHAVIOR_BEHAVIORRUNTIME_H

#include <flat.h>
#include <memory>

namespace game
{
namespace entity
{
class Entity;
namespace behavior
{
class Behavior;

class BehaviorRuntime final
{
	public:
		BehaviorRuntime(Entity* entity);
		BehaviorRuntime(const BehaviorRuntime&) = delete;
		void operator=(const BehaviorRuntime&) = delete;
		~BehaviorRuntime();
		
		void enterState(const char* stateName);
		void enterState(lua_State* L, int index);
		void updateCurrentState();
		void update();
		
	private:
		const Behavior* getBehavior() const;
		
	private:
		Entity* m_entity;
		int m_coroutineRef;
};

} // behavior
} // entity
} // game

#endif // GAME_ENTITY_BEHAVIOR_BEHAVIORRUNTIME_H



