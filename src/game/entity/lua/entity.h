#ifndef GAME_ENTITY_LUA_ENTITY_H
#define GAME_ENTITY_LUA_ENTITY_H

#include <flat.h>

namespace game
{
namespace entity
{
class Entity;
namespace lua
{

int open(lua_State* L);

int l_Entity_getTemplateName(lua_State* L);

int l_Entity_despawn(lua_State* L);

int l_Entity_getExtraData(lua_State* L);

int l_Entity_setPosition(lua_State* L);
int l_Entity_getPosition(lua_State* L);

int l_Entity_setHeading(lua_State* L);
int l_Entity_getHeading(lua_State* L);

int l_Entity_setElevation(lua_State* L);
int l_Entity_getElevation(lua_State* L);

int l_Entity_moveTo(lua_State* L);
int l_Entity_clearPath(lua_State* L);
int l_Entity_setSpeed(lua_State* L);
int l_Entity_enterState(lua_State* L);
int l_Entity_playAnimation(lua_State* L);
int l_Entity_jump(lua_State* L);
int l_Entity_setMoveAnimation(lua_State* L);
int l_Entity_setDefaultMoveAnimation(lua_State* L);
int l_Entity_canSee(lua_State* L);

// static lua functions
int l_Entity_spawn(lua_State* L);

// private
Entity& getEntity(lua_State* L, int index);
void pushEntity(lua_State* L, Entity* entity);

template <class T>
T& getComponent(lua_State* L, Entity& entity)
{
	T* component = entity.getComponent<T>();
	if (!component)
	{
		luaL_error(L, "%s has no %s component", entity.getTemplateName().c_str(), T::getConfigName());
	}
	return *component;
}

} // lua
} // entity
} // game

#endif // GAME_ENTITY_LUA_ENTITY_H



