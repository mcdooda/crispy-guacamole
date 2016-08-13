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

int l_Entity_getPosition(lua_State* L);
int l_Entity_moveTo(lua_State* L);
int l_Entity_enterState(lua_State* L);
int l_Entity_playAnimation(lua_State* L);

// private
Entity* getEntity(lua_State* L, int index);
void pushEntity(lua_State* L, Entity* entity);

} // lua
} // entity
} // game

#endif // GAME_ENTITY_LUA_ENTITY_H



