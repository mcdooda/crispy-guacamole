#ifndef GAME_MAP_BRUSH_LUA_TILESCONTAINER_H
#define GAME_MAP_BRUSH_LUA_TILESCONTAINER_H

#include "../brush.h"

namespace game
{
namespace map
{
namespace brush
{
namespace lua
{
int openTilesContainer(flat::Flat& flat);

int l_TilesContainer_eachTile(lua_State* L);

// private
TilesContainer& getTilesContainer(lua_State* L, int index);
void pushTilesContainer(lua_State* L, TilesContainer* tilesContainer);

} // lua
} // brush
} // map
} // game

#endif // GAME_MAP_BRUSH_LUA_TILESCONTAINER_H


