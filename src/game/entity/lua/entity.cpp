#include "entity.h"
#include "../entity.h"

namespace game
{
namespace entity
{
namespace lua
{

int open(lua_State* L)
{
	// Entity metatable
	luaL_newmetatable(L, "CG.Entity");
	// mt.__index = mt
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	
	static const luaL_Reg Entity_lib_m[] = {
		{"getPosition", l_Entity_getPosition},
		{"moveTo",      l_Entity_moveTo},
		{"enterState",  l_Entity_enterState},
		
		{nullptr, nullptr}
	};
	
	luaL_setfuncs(L, Entity_lib_m, 0);
	
	lua_pop(L, 1);
	
	return 0;
}

int l_Entity_getPosition(lua_State* L)
{
	Entity* entity = getEntity(L, 1);
	const flat::geometry::Vector3& position = entity->getPosition();
	lua_pushnumber(L, position.x);
	lua_pushnumber(L, position.y);
	lua_pushnumber(L, position.z);
	return 3;
}

int l_Entity_moveTo(lua_State* L)
{
	Entity* entity = getEntity(L, 1);
	float x = luaL_checknumber(L, 2);
	float y = luaL_checknumber(L, 3);
	entity->addPointOnPath(flat::geometry::Vector2(x, y));
	return lua_yield(L, 0);
}

int l_Entity_enterState(lua_State* L)
{
	Entity* entity = getEntity(L, 1);
	/*luaL_checktype(L, 2, LUA_TFUNCTION);
	entity->enterState(L, 2);*/
	const char* stateName = luaL_checkstring(L, 2);
	entity->enterState(stateName);
	return lua_yield(L, 0);
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



