#ifndef GAME_ENTITY_COMPONENT_LUA_COMPONENTREGISTRY_H
#define GAME_ENTITY_COMPONENT_LUA_COMPONENTREGISTRY_H

#include <flat.h>

namespace game
{
namespace entity
{
namespace component
{
namespace lua
{

int open(lua_State* L);

int l_ComponentRegistry_getConfigNames(lua_State* L);

} // lua
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_LUA_COMPONENTREGISTRY_H


