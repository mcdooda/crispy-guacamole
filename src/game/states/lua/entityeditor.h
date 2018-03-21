#ifndef GAME_STATES_LUA_ENTITYEDITOR_H
#define GAME_STATES_LUA_ENTITYEDITOR_H

#include <flat.h>

namespace game
{
namespace states
{
namespace lua
{
namespace entityeditor
{

int open(lua_State* L);

int l_EntityEditor_newEntity(lua_State* L);
int l_EntityEditor_openEntity(lua_State* L);

int l_EntityEditor_entitySpawned(lua_State* L);
int l_EntityEditor_entityDespawned(lua_State* L);

} // entityeditor
} // lua
} // states
} // game

#endif // GAME_STATES_LUA_ENTITYEDITOR_H


