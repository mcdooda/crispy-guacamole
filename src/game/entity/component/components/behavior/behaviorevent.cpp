#include "behaviorevent.h"
#include "../../../lua/entity.h"

namespace game
{
namespace entity
{
namespace component
{
namespace behavior
{

int EntityEnteredVisionRangeBehaviorEvent::push(lua_State* L, Entity* entity)
{
	lua::pushEntity(L, entity);
	return 1;
}

int EntityLeftVisionRangeBehaviorEvent::push(lua_State* L, Entity* entity)
{
	lua::pushEntity(L, entity);
	return 1;
}

int DamageTakenBehaviorEvent::push(lua_State* L, int amount, Entity* instigator)
{
	lua_pushinteger(L, amount);
	lua::pushEntity(L, instigator);
	return 2;
}

int PlayerMoveOrderEvent::push(lua_State* L, const flat::Vector2& destination, Entity* interactionEntity)
{
	flat::lua::pushVector2(L, destination);
	lua::pushEntity(L, interactionEntity);
	return 2;
}

} // behavior
} // component
} // entity
} // game




