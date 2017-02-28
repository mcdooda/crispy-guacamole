#ifndef GAME_ENTITY_COMPONENT_LUA_COMPONENTREGISTRY_H
#define GAME_ENTITY_COMPONENT_LUA_COMPONENTREGISTRY_H

#include <flat.h>

namespace game
{
namespace entity
{
namespace component
{
class ComponentRegistry;
namespace lua
{

int open(lua_State* L, const ComponentRegistry& componentRegistry);

int l_Components_allExcept(lua_State* L);

// private
int setComponentFlagsTable(lua_State* L, const ComponentRegistry& componentRegistry);

} // lua
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_LUA_COMPONENTREGISTRY_H


