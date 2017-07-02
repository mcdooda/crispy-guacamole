#ifndef GAME_STATES_EDITOR_LUA_EDITOR_H
#define GAME_STATES_EDITOR_LUA_EDITOR_H

#include <flat.h>
#include "../../../game.h"
#include "../../mapeditorstate.h"

namespace game
{
namespace states
{
namespace editor
{
class EditorMode;
namespace lua
{

int open(lua_State* L);

int l_Editor_setEntityMode(lua_State* L);
int l_Editor_setEntity(lua_State* L);

int l_Editor_setTileMode(lua_State* L);
int l_Editor_setTile(lua_State* L);

int l_Editor_setPropMode(lua_State* L);
int l_Editor_setProp(lua_State* L);

int l_Editor_setZoneMode(lua_State* L);
int l_Editor_setZone(lua_State* L);
int l_Editor_addZone(lua_State* L);
int l_Editor_getZoneNames(lua_State* L);

int l_Editor_getBrushPosition(lua_State* L);

// private
states::MapEditorState& getEditorState(lua_State* L);

template <class T>
void setEditorMode(lua_State* L)
{
	static_assert(std::is_base_of<editor::EditorMode, T>::value, "T must inherit from editor::EditorMode");
	Game& game = flat::lua::getGame(L).to<Game>();
	editor::EditorMode* editorMode = new T(game);
	states::MapEditorState& editorState = getEditorState(L);
	editorState.setEditorMode(editorMode);
}

editor::EditorMode& getEditorMode(lua_State* L);

} // lua
} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_LUA_EDITOR_H


