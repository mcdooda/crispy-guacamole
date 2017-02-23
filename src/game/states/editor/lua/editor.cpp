#include "editor.h"
#include "../../../game.h"
#include "../../editorstate.h"
#include "../entityeditormode.h"
#include "../propeditormode.h"
#include "../tileeditormode.h"
#include "../zoneeditormode.h"

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
		{"setEntityMode",    l_Editor_setEntityMode},
		{"setEntity",        l_Editor_setEntity},

		{"setTileMode",      l_Editor_setTileMode},
		{"setTile",          l_Editor_setTile},

		{"setPropMode",      l_Editor_setPropMode},
		{"setProp",          l_Editor_setProp},

		{"setZoneMode",      l_Editor_setZoneMode},
		{"setZone",          l_Editor_setZone},
		{"addZone",          l_Editor_addZone},
		{"getZoneNames",     l_Editor_getZoneNames},

		{"getBrushPosition", l_Editor_getBrushPosition},

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
	editor::EntityEditorMode& entityEditorMode = getEditorMode(L).to<editor::EntityEditorMode>();
	Game& game = flat::lua::getGame(L).to<Game>();
	states::EditorState& editorState = getEditorState(L);
	std::shared_ptr<const entity::EntityTemplate> entityTemplate = editorState.getEntityTemplate(game, entityTemplateName);
	entityEditorMode.setEntityTemplate(entityTemplate);
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
	editor::TileEditorMode& tileEditorMode = getEditorMode(L).to<editor::TileEditorMode>();
	Game& game = flat::lua::getGame(L).to<Game>();
	states::EditorState& editorState = getEditorState(L);
	std::shared_ptr<const map::TileTemplate> tileTemplate = editorState.getTileTemplate(game, tileTemplateName);
	tileEditorMode.setTileTemplate(tileTemplate);
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
	editor::PropEditorMode& propEditorMode = getEditorMode(L).to<editor::PropEditorMode>();
	Game& game = flat::lua::getGame(L).to<Game>();
	states::EditorState& editorState = getEditorState(L);
	std::shared_ptr<const map::PropTemplate> propTemplate = editorState.getPropTemplate(game, propTemplateName);
	propEditorMode.setPropTemplate(propTemplate);
	return 0;
}

int l_Editor_setZoneMode(lua_State * L)
{
	setEditorMode<editor::ZoneEditorMode>(L);
	return 0;
}

int l_Editor_setZone(lua_State* L)
{
	const char* zoneName = luaL_checkstring(L, 1);
	editor::ZoneEditorMode& zoneEditorMode = getEditorMode(L).to<editor::ZoneEditorMode>();
	states::EditorState& editorState = getEditorState(L);
	const map::Map& map = editorState.getMap();
	std::shared_ptr<map::Zone> zone;
	if (map.getZone(zoneName, zone))
	{
		zoneEditorMode.setCurrentZone(zone);
		lua_pushboolean(L, 1);
	}
	else
	{
		lua_pushboolean(L, 0);
	}
	return 1;
}

int l_Editor_addZone(lua_State* L)
{
	const char* zoneName = luaL_checkstring(L, 1);
	editor::ZoneEditorMode& zoneEditorMode = getEditorMode(L).to<editor::ZoneEditorMode>();
	zoneEditorMode.addZone(zoneName);
	return 0;
}

int l_Editor_getZoneNames(lua_State* L)
{
	states::EditorState& editorState = getEditorState(L);
	const map::Map& map = editorState.getMap();
	const std::map<std::string, std::shared_ptr<map::Zone>>& zones = map.getZones();
	int numZones = static_cast<int>(zones.size());
	lua_createtable(L, numZones, 0);
	int i = 1;
	for (const std::pair<std::string, std::shared_ptr<map::Zone>>& pair : zones)
	{
		lua_pushstring(L, pair.first.c_str());
		lua_rawseti(L, -2, i);
		++i;
	}
	return 1;
}

int l_Editor_getBrushPosition(lua_State * L)
{
	states::EditorState& editorState = getEditorState(L);
	editor::EditorMode* editorMode = editorState.getEditorMode();
	if (editorMode != nullptr) // editorMode not ready yet? TODO: fix this
	{
		flat::Vector2 brushPosition = editorMode->getBrushPosition();
		lua_pushnumber(L, brushPosition.x);
		lua_pushnumber(L, brushPosition.y);
	}
	else
	{
		lua_pushnumber(L, 0.f);
		lua_pushnumber(L, 0.f);
	}
	return 2;
}

// private
states::EditorState& getEditorState(lua_State * L)
{
	Game& game = flat::lua::getGame(L).to<Game>();
	flat::state::State* state = game.getStateMachine().getState();
	return state->as<states::EditorState>();
}

editor::EditorMode& getEditorMode(lua_State* L)
{
	states::EditorState& editorState = getEditorState(L);
	return *editorState.getEditorMode();
}

} // lua
} // editor
} // states
} // game


