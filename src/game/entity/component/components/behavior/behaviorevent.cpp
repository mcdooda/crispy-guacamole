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

int PlayerMoveOrderEvent::push(lua_State* L, Entity* entity, const flat::Vector2& destination, Entity* interactionEntity)
{
	lua::pushEntity(L, entity);
	flat::lua::pushVector2(L, destination);
	lua::pushEntity(L, interactionEntity);
	return 3;
}

} // behavior
} // component
} // entity
} // game




