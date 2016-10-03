#include "editor.h"
#include "../../../game.h"
#include "../../editorstate.h"
#include "../entityeditormode.h"
#include "../propeditormode.h"
#include "../tileeditormode.h"

namespace game
{
namespace states
{
namespace editor
{
namespace lua
{

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_createtable(L, 0, 6);
	static const luaL_Reg Editor_lib_m[] = {
		{"setEntityMode", l_Editor_setEntityMode},
		{"setEntity",     l_Editor_setEntity},
		{"setTileMode",   l_Editor_setTileMode},
		{"setTile",       l_Editor_setTile},
		{"setPropMode",   l_Editor_setPropMode},
		{"setProp",       l_Editor_setProp},
		{nullptr, nullptr}
	};
	luaL_setfuncs(L, Editor_lib_m, 0);
	lua_setglobal(L, "Editor");

	return 0;
}

int l_Editor_setEntityMode(lua_State * L)
{
	setEditorMode<editor::EntityEditorMode>(L);
	return 0;
}

int l_Editor_setEntity(lua_State * L)
{
	const char* entityTemplateName = luaL_checkstring(L, 1);
	editor::EntityEditorMode* entityEditorMode = getEditorMode<editor::EntityEditorMode>(L);
	Game* game = flat::lua::getGameAs<Game>(L);
	states::EditorState* editorState = getEditorState(L);
	std::shared_ptr<const entity::EntityTemplate> entityTemplate = editorState->getEntityTemplate(game, entityTemplateName);
	entityEditorMode->setEntityTemplate(entityTemplate);
	return 0;
}

int l_Editor_setTileMode(lua_State * L)
{
	setEditorMode<editor::TileEditorMode>(L);
	return 0;
}

int l_Editor_setTile(lua_State * L)
{
	const char* tileTemplateName = luaL_checkstring(L, 1);
	editor::TileEditorMode* tileEditorMode = getEditorMode<editor::TileEditorMode>(L);
	Game* game = flat::lua::getGameAs<Game>(L);
	states::EditorState* editorState = getEditorState(L);
	std::shared_ptr<const map::TileTemplate> tileTemplate = editorState->getTileTemplate(game, tileTemplateName);
	tileEditorMode->setTileTemplate(tileTemplate);
	return 0;
}

int l_Editor_setPropMode(lua_State * L)
{
	setEditorMode<editor::PropEditorMode>(L);
	return 0;
}

int l_Editor_setProp(lua_State * L)
{
	const char* propTemplateName = luaL_checkstring(L, 1);
	editor::PropEditorMode* propEditorMode = getEditorMode<editor::PropEditorMode>(L);
	Game* game = flat::lua::getGameAs<Game>(L);
	states::EditorState* editorState = getEditorState(L);
	std::shared_ptr<const map::PropTemplate> propTemplate = editorState->getPropTemplate(game, propTemplateName);
	propEditorMode->setPropTemplate(propTemplate);
	return 0;
}

// private
states::EditorState* getEditorState(lua_State * L)
{
	Game* game = flat::lua::getGameAs<Game>(L);
	flat::state::State* state = game->getStateMachine().getCurrentState();
	FLAT_ASSERT(dynamic_cast<states::EditorState*>(state) != nullptr);
	return static_cast<states::EditorState*>(state);
}

} // lua
} // editor
} // states
} // game


