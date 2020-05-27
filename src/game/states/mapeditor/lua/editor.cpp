#include "editor.h"
#include "game.h"
#include "states/mapeditorstate.h"
#include "states/mapeditor/entitymapeditormode.h"
#include "states/mapeditor/propmapeditormode.h"
#include "states/mapeditor/tilemapeditormode.h"
#include "states/mapeditor/zonemapeditormode.h"

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

	lua_createtable(L, 0, 9);
	static const luaL_Reg Editor_lib_m[] = {
		{"clearSelectedAsset", l_Editor_clearSelectedAsset},

		{"setEntity",          l_Editor_setEntity},
		{"setTile",            l_Editor_setTile},
		{"setProp",            l_Editor_setProp},

		{"setZone",            l_Editor_setZone},
		{"addZone",            l_Editor_addZone},
		{"removeZone",         l_Editor_removeZone},
		{"getZoneNames",       l_Editor_getZoneNames},

		{"getBrushPosition", l_Editor_getBrushPosition},

		{nullptr, nullptr}
	};
	luaL_setfuncs(L, Editor_lib_m, 0);
	lua_setglobal(L, "Editor");

	return 0;
}

int l_Editor_clearSelectedAsset(lua_State* L)
{
	clearEditorMode(L);
	return 0;
}

int l_Editor_setEntityMode(lua_State* L)
{
	setEditorMode<editor::EntityMapEditorMode>(L);
	return 0;
}

int l_Editor_setEntity(lua_State* L)
{
	const char* entityTemplateName = luaL_checkstring(L, 1);
	editor::EntityMapEditorMode& entityEditorMode = setEditorMode<editor::EntityMapEditorMode>(L);
	Game& game = flat::lua::getFlatAs<Game>(L);
	states::MapEditorState& mapEditorState = getEditorState(L);
	std::shared_ptr<const entity::EntityTemplate> entityTemplate = mapEditorState.getEntityTemplate(game, entityTemplateName);
	entityEditorMode.setEntityTemplate(mapEditorState, entityTemplate);
	return 0;
}

int l_Editor_setTile(lua_State* L)
{
	const char* tileTemplatePath = luaL_checkstring(L, 1);
	editor::TileMapEditorMode& tileEditorMode = setEditorMode<editor::TileMapEditorMode>(L);
	Game& game = flat::lua::getFlatAs<Game>(L);
	states::MapEditorState& editorState = getEditorState(L);
	std::shared_ptr<const map::TileTemplate> tileTemplate = editorState.getTileTemplate(game, tileTemplatePath);
	tileEditorMode.setTileTemplate(tileTemplate);
	return 0;
}

int l_Editor_setProp(lua_State* L)
{
	const char* propTemplatePath = luaL_checkstring(L, 1);
	editor::PropMapEditorMode& propEditorMode = setEditorMode<editor::PropMapEditorMode>(L);
	Game& game = flat::lua::getFlatAs<Game>(L);
	states::MapEditorState& editorState = getEditorState(L);
	std::shared_ptr<const map::PropTemplate> propTemplate = editorState.getPropTemplate(game, propTemplatePath);
	propEditorMode.setPropTemplate(propTemplate);
	return 0;
}

int l_Editor_setZone(lua_State* L)
{
	const char* zoneName = luaL_checkstring(L, 1);
	editor::ZoneMapEditorMode& zoneEditorMode = setEditorMode<editor::ZoneMapEditorMode>(L);
	states::MapEditorState& mapEditorState = getEditorState(L);
	const map::Map& map = mapEditorState.getMap();
	std::shared_ptr<map::Zone> zone;
	if (map.getZone(zoneName, zone))
	{
		zoneEditorMode.setCurrentZone(mapEditorState, zone);
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
	editor::ZoneMapEditorMode& zoneEditorMode = setEditorMode<editor::ZoneMapEditorMode>(L);
	states::MapEditorState& mapEditorState = getEditorState(L);
	zoneEditorMode.addZone(mapEditorState, zoneName);
	return 0;
}

int l_Editor_removeZone(lua_State * L)
{
	const char* zoneName = luaL_checkstring(L, 1);
	editor::ZoneMapEditorMode& zoneEditorMode = setEditorMode<editor::ZoneMapEditorMode>(L);
	states::MapEditorState& mapEditorState = getEditorState(L);
	zoneEditorMode.removeZone(mapEditorState, zoneName);
	return 0;
}

int l_Editor_getZoneNames(lua_State* L)
{
	states::MapEditorState& editorState = getEditorState(L);
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

int l_Editor_getBrushPosition(lua_State* L)
{
	states::MapEditorState& editorState = getEditorState(L);
	editor::MapEditorMode* editorMode = editorState.getEditorMode();
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
states::MapEditorState& getEditorState(lua_State* L)
{
	Game& game = flat::lua::getFlatAs<Game>(L);
	flat::state::State* state = game.getStateMachine().getState();
	return state->as<states::MapEditorState>();
}

void clearEditorMode(lua_State* L)
{
	states::MapEditorState& editorState = getEditorState(L);
	editorState.clearEditorMode();
}

editor::MapEditorMode& getEditorMode(lua_State* L)
{
	states::MapEditorState& editorState = getEditorState(L);
	return *editorState.getEditorMode();
}

} // lua
} // editor
} // states
} // game


