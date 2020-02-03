#ifndef GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIOREVENT_H
#define GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIOREVENT_H

#include <flat.h>

namespace game
{
namespace entity
{
class Entity;
namespace component
{
namespace behavior
{

class BehaviorEvent
{
	public:
		inline static const char* getMethodName() { FLAT_ASSERT_MSG(false, "BehaviorEventType::getMethodName() missing"); return nullptr; }
};

class EntityEnteredVisionRangeBehaviorEvent : public BehaviorEvent
{
	public:
		inline static const char* getMethodName() { return "onEntityEnteredVisionRange"; }

		static int push(lua_State* L, Entity* entity);
};

class EntityLeftVisionRangeBehaviorEvent : public BehaviorEvent
{
	public:
		inline static const char* getMethodName() { return "onEntityLeftVisionRange"; }

		static int push(lua_State* L, Entity* entity);
};

class DamageTakenBehaviorEvent : public BehaviorEvent
{
	public:
		inline static const char* getMethodName() { return "onDamageTaken"; }

		static int push(lua_State* L, int amount, Entity* instigator);
};

class PlayerMoveOrderEvent : public BehaviorEvent
{
	public:
		inline static const char* getMethodName() { return "onPlayerMoveOrder"; }

		static int push(lua_State* L, const flat::Vector2& destination, Entity* interactionEntity);
};

} // behavior
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIOREVENT_H


