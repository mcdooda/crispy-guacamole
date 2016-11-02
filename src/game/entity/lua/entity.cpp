#include "entity.h"
#include "../entity.h"
#include "../component/components/movement/movementcomponent.h"
#include "../../game.h"
#include "../../states/basemapstate.h"

namespace game
{
namespace entity
{
namespace lua
{

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	// Entity metatable
	luaL_newmetatable(L, "CG.Entity");
	// mt.__index = mt
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	
	static const luaL_Reg Entity_lib_m[] = {
		{"setPosition",   l_Entity_setPosition},
		{"getPosition",   l_Entity_getPosition},

		{"setHeading",    l_Entity_setHeading},
		{"getHeading",    l_Entity_getHeading},

		{"moveTo",        l_Entity_moveTo},
		{"enterState",    l_Entity_enterState},
		{"playAnimation", l_Entity_playAnimation},
		{"jump",          l_Entity_jump},
		
		{nullptr, nullptr}
	};
	luaL_setfuncs(L, Entity_lib_m, 0);
	
	lua_createtable(L, 0, 1);
	static const luaL_Reg Entity_lib_f[] = {
		{"spawn", l_Entity_spawn},
		{nullptr, nullptr}
	};
	luaL_setfuncs(L, Entity_lib_f, 0);
	lua_setglobal(L, "Entity");

	lua_pop(L, 1);
	
	return 0;
}

int l_Entity_setPosition(lua_State* L)
{
	Entity* entity = getEntity(L, 1);
	float x = static_cast<float>(luaL_checknumber(L, 2));
	float y = static_cast<float>(luaL_checknumber(L, 3));
	float z = static_cast<float>(luaL_checknumber(L, 4));
	flat::Vector3 position(x, y, z);
	entity->setPosition(position);
	return 0;
}

int l_Entity_getPosition(lua_State* L)
{
	Entity* entity = getEntity(L, 1);
	const flat::Vector3& position = entity->getPosition();
	lua_pushnumber(L, position.x);
	lua_pushnumber(L, position.y);
	lua_pushnumber(L, position.z);
	return 3;
}

int l_Entity_setHeading(lua_State* L)
{
	Entity* entity = getEntity(L, 1);
	float heading = static_cast<float>(luaL_checknumber(L, 2));
	entity->setHeading(heading);
	return 0;
}

int l_Entity_getHeading(lua_State* L)
{
	Entity* entity = getEntity(L, 1);
	lua_pushnumber(L, entity->getHeading());
	return 1;
}

int l_Entity_moveTo(lua_State* L)
{
	Entity* entity = getEntity(L, 1);
	float x = static_cast<float>(luaL_checknumber(L, 2));
	float y = static_cast<float>(luaL_checknumber(L, 3));
	entity->addPointOnPath(flat::Vector2(x, y));
	return lua_yield(L, 0);
}

int l_Entity_enterState(lua_State* L)
{
	Entity* entity = getEntity(L, 1);
	const char* stateName = luaL_checkstring(L, 2);
	entity->enterState(stateName);
	return lua_yield(L, 0);
}

int l_Entity_playAnimation(lua_State* L)
{
	Entity* entity = getEntity(L, 1);
	const char* animationName = luaL_checkstring(L, 2);
	int numLoops = luaL_optint(L, 3, 1);
	bool plays = entity->playAnimation(animationName, numLoops);
	if (!plays)
	{
		luaL_error(L, "Animation %s does not exist", animationName);
	}
	return lua_yield(L, 0);
}

int l_Entity_jump(lua_State* L)
{
	Entity* entity = getEntity(L, 1);
	component::movement::MovementComponent* movementComponent = entity->getComponent<component::movement::MovementComponent>();
	if (!movementComponent)
	{
		luaL_error(L, "Cannot jump without a movement component");
	}
	if (!movementComponent->isTouchingGround())
	{
		luaL_error(L, "Cannot jump midair");
	}
	movementComponent->jump();
	return lua_yield(L, 0);
}

// static lua functions
int l_Entity_spawn(lua_State* L)
{
	const char* entityTemplateName = luaL_checkstring(L, 1);
	float x = static_cast<float>(luaL_checknumber(L, 2));
	float y = static_cast<float>(luaL_checknumber(L, 3));
	float z = static_cast<float>(luaL_checknumber(L, 4));
	flat::Vector3 position(x, y, z);
	float heading = static_cast<float>(luaL_checknumber(L, 5));
	Game& game = flat::lua::getGame(L).to<Game>();
	states::BaseMapState& baseMapState = game.getStateMachine().getState()->to<states::BaseMapState>();
	const std::shared_ptr<const EntityTemplate>& entityTemplate = baseMapState.getEntityTemplate(game, entityTemplateName);
	Entity* entity = baseMapState.spawnEntityAtPosition(entityTemplate, position);
	entity->setHeading(heading);
	return 1;
}

// private
Entity* getEntity(lua_State* L, int index)
{
	return *static_cast<Entity**>(luaL_checkudata(L, index, "CG.Entity"));
}

void pushEntity(lua_State* L, Entity* entity)
{
	if (entity != nullptr)
	{
		Entity** entityPointer = static_cast<Entity**>(lua_newuserdata(L, sizeof(Entity*)));
		*entityPointer = entity;
		luaL_getmetatable(L, "CG.Entity");
		lua_setmetatable(L, -2);
	}
	else
	{
		lua_pushnil(L);
	}
}

} // lua
} // entity
} // game



