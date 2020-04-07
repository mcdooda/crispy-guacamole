#ifndef GAME_MOD_LUA_MOD_H
#define GAME_MOD_LUA_MOD_H

#include <flat.h>

namespace game
{
namespace mod
{
namespace lua
{

int open(lua_State* L);

int l_Mod_getPath(lua_State* L);
int l_Mod_getCurrentMapName(lua_State* L);

int l_Mod_getEntities(lua_State* L);
int l_Mod_getMaps(lua_State* L);
int l_Mod_getProps(lua_State* L);
int l_Mod_getTiles(lua_State* L);

int l_Mod_openEditor(lua_State* L);

int l_Mod_getFilePath(lua_State* L);

} // lua
} // mod
} // game

#endif // GAME_MOD_LUA_MOD_H


