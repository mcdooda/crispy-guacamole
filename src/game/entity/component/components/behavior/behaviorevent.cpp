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

int EntityEnteredVisionRangeBehaviorEvent::push(lua_State* L, entity::Entity* entity)
{
	lua::pushEntity(L, entity);
	return 1;
}

int EntityLeftVisionRangeBehaviorEvent::push(lua_State* L, entity::Entity* entity)
{
	lua::pushEntity(L, entity);
	return 1;
}

} // behavior
} // component
} // entity
} // game




