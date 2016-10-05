#ifndef GAME_STATES_EDITOR_LUA_EDITOR_H
#define GAME_STATES_EDITOR_LUA_EDITOR_H

#include <flat.h>

namespace game
{
namespace states
{
class EditorState;
namespace editor
{
namespace lua
{

int open(lua_State* L);

int l_Editor_setEntityMode(lua_State* L);
int l_Editor_setEntity(lua_State* L);

int l_Editor_setTileMode(lua_State* L);
int l_Editor_setTile(lua_State* L);

int l_Editor_setPropMode(lua_State* L);
int l_Editor_setProp(lua_State* L);

// private
states::EditorState* getEditorState(lua_State* L);

template <class T>
void setEditorMode(lua_State* L)
{
	static_assert(std::is_base_of<editor::EditorMode, T>::value, "T must inherit from editor::EditorMode");
	states::EditorState* editorState = getEditorState(L);
	Game& game = flat::lua::getGameAs<Game>(L);
	editor::EditorMode* editorMode = new T(game, *editorState);
	editorState->setEditorMode(editorMode);
}

template <class T>
T* getEditorMode(lua_State* L)
{
	static_assert(std::is_base_of<editor::EditorMode, T>::value, "T must inherit from editor::EditorMode");
	states::EditorState* editorState = getEditorState(L);
	editor::EditorMode* editorMode = editorState->getEditorMode();
	FLAT_ASSERT(dynamic_cast<T*>(editorMode) != nullptr);
	return static_cast<T*>(editorMode);
}

} // lua
} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_LUA_EDITOR_H


