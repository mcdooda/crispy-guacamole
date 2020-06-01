#ifndef GAME_STATES_EDITOR_LUA_EDITOR_H
#define GAME_STATES_EDITOR_LUA_EDITOR_H

#include <flat.h>
#include "game.h"
#include "states/mapeditorstate.h"

namespace game
{
namespace states
{
namespace editor
{
class MapEditorMode;
namespace lua
{

int open(lua_State* L);

int l_Editor_clearSelectedAsset(lua_State* L);

int l_Editor_setEntity(lua_State* L);
int l_Editor_setTile(lua_State* L);
int l_Editor_setProp(lua_State* L);

int l_Editor_setZone(lua_State* L);
int l_Editor_addZone(lua_State* L);
int l_Editor_removeZone(lua_State* L);
int l_Editor_getZoneNames(lua_State* L);

int l_Editor_getBrushPosition(lua_State* L);

// private
states::MapEditorState& getEditorState(lua_State* L);

template <class T>
T& setEditorMode(lua_State* L)
{
	static_assert(std::is_base_of<editor::MapEditorMode, T>::value, "T must inherit from editor::MapEditorMode");
	Game& game = flat::lua::getFlatAs<Game>(L);

	states::MapEditorState& mapEditorState = getEditorState(L);
	editor::MapEditorMode* mapEditorMode = mapEditorState.getEditorMode();
	if (mapEditorMode != nullptr && mapEditorMode->is<T>())
	{
		return mapEditorMode->to<T>();
	}

	std::unique_ptr<editor::MapEditorMode> newEditorMode = std::make_unique<T>(game);
	mapEditorState.setEditorMode(std::move(newEditorMode));
	return mapEditorState.getEditorMode()->to<T>();
}

void clearEditorMode(lua_State* L);
editor::MapEditorMode& getEditorMode(lua_State* L);

} // lua
} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_LUA_EDITOR_H


