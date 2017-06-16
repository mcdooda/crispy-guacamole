#ifndef GAME_MAP_BRUSH_LUA_BRUSH_H
#define GAME_MAP_BRUSH_LUA_BRUSH_H

struct lua_State;

namespace game
{
namespace map
{
namespace brush
{
class Brush;
namespace lua
{

int open(lua_State* L);

int l_Brush_setRadius(lua_State* L);
int l_Brush_setEdgeWidth(lua_State* L);

int l_Brush_getTiles(lua_State* L);

int l_Brush_cone(lua_State* L);
int l_Brush_sphere(lua_State* L);

// private
Brush& getBrush(lua_State* L, int index);

} // lua
} // brush
} // map
} // game

#endif // GAME_MAP_BRUSH_LUA_BRUSH_H


