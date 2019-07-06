#include "map.h"
#include "zone.h"
#include "../tile.h"
#include "../../game.h"
#include "../../states/mapeditorstate.h"
#include "../../entity/lua/entity.h"

namespace game
{
namespace map
{
namespace lua
{
namespace map
{

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_createtable(L, 0, 3);
	static const luaL_Reg Map_lib_m[] = {
		{"getName",                       l_Map_getName},
		{"load",                          l_Map_load},
		{"save",                          l_Map_save},

#ifdef FLAT_DEBUG
		{"debug_getDrawStats",            l_Map_debug_getDrawStats},
		{"debug_enableNavigabilityDebug", l_Map_debug_enableNavigabilityDebug},
		{"debug_enableTileIndicesDebug",  l_Map_debug_enableTileIndicesDebug},
#endif

		{"getNumEntities",                l_Map_getNumEntities},
		{"getEntitiesInRange",            l_Map_getEntitiesInRange},
		{"eachSelectedEntity",            l_Map_eachSelectedEntity},
		{"getEntitiesOfType",             l_Map_getEntitiesOfType},

		{"getZone",                       l_Map_getZone},

		{"getTilePosition",               l_Map_getTilePosition},
		{"getTileZ",                      l_Map_getTileZ},
		{"setTileZ",                      l_Map_setTileZ},
		{"moveTileZBy",                   l_Map_moveTileZBy},

		{nullptr, nullptr}
	};
	luaL_setfuncs(L, Map_lib_m, 0);

	lua_pushstring(L, "Navigability");
	static const flat::lua::table::KeyValuePair<int> navigabilityTable[] = {
		{"NONE",   Navigability::NONE},
		{"GROUND", Navigability::GROUND},
		{"WATER",  Navigability::WATER},
		{"ALL",    Navigability::ALL},

		{nullptr, 0},
	};
	flat::lua::table::pushTable(L, navigabilityTable);
	lua_settable(L, -3);

	lua_setglobal(L, "Map");
	
	return 0;
}

int l_Map_getName(lua_State* L)
{
	lua_pushstring(L, "map1");
	return 1;
}

int l_Map_load(lua_State* L)
{
	std::string mapName = luaL_checkstring(L, 1);
	Game& game = flat::lua::getFlatAs<Game>(L);
	game.mapName = mapName;
	flat::state::State* state = game.getStateMachine().getState();
	states::BaseMapState& baseMapState = state->as<states::BaseMapState>();
	bool mapLoaded = baseMapState.loadMap(game);
	lua_pushboolean(L, mapLoaded);
	return 1;
}

int l_Map_save(lua_State* L)
{
	Game& game = flat::lua::getFlatAs<Game>(L);
	flat::state::State* state = game.getStateMachine().getState();
	states::MapEditorState& editorState = state->as<states::MapEditorState>();
	bool mapSaved = editorState.saveMap(game);
	lua_pushboolean(L, mapSaved);
	return 1;
}

#ifdef FLAT_DEBUG
int l_Map_debug_getDrawStats(lua_State* L)
{
	const Map& map = getMap(L);
	const DisplayManager& displayManager = map.getDisplayManager();
	lua_pushnumber(L, static_cast<lua_Number>(displayManager.getNumOpaqueObjects()));
	lua_pushnumber(L, static_cast<lua_Number>(displayManager.getNumOpaqueDrawCalls()));
	lua_pushnumber(L, static_cast<lua_Number>(displayManager.getNumTransparentObjects()));
	lua_pushnumber(L, static_cast<lua_Number>(displayManager.getNumTransparentDrawCalls()));
	return 4;
}

int l_Map_debug_enableNavigabilityDebug(lua_State* L)
{
	bool enable = lua_toboolean(L, 1) == 1;
	Map& map = getMap(L);
	map.enableNavigabilityDebug(enable);
	return 0;
}

int l_Map_debug_enableTileIndicesDebug(lua_State* L)
{
	bool enable = lua_toboolean(L, 1) == 1;
	Map& map = getMap(L);
	map.enableTileIndicesDebug(enable);
	return 0;
}
#endif // FLAT_DEBUG

int l_Map_getNumEntities(lua_State* L)
{
	Game& game = flat::lua::getFlatAs<Game>(L);
	flat::state::State* state = game.getStateMachine().getState();
	states::BaseMapState& baseMapState = state->as<states::BaseMapState>();
	lua_pushinteger(L, baseMapState.getEntityUpdater().getEntities().size());
	return 1;
}

int l_Map_getEntitiesInRange(lua_State* L)
{
	flat::Vector2& position = flat::lua::getVector2(L, 1);
	float range = static_cast<float>(luaL_checknumber(L, 2));
	const Map& map = getMap(L);
	lua_newtable(L);
	int i = 1;
	map.eachEntityInRange(position, range, [L, &i](entity::Entity* entity)
	{
		entity::lua::pushEntity(L, entity);
		lua_rawseti(L, -2, i);
		++i;
	});
	return 1;
}

namespace
{
static int locIterateOverSelectedEntities(lua_State*L)
{
	luaL_checktype(L, 1, LUA_TNIL);

	Game& game = flat::lua::getFlatAs<Game>(L);
	flat::state::State* state = game.getStateMachine().getState();
	states::BaseMapState& baseMapState = state->as<states::BaseMapState>();
	const std::vector<entity::Entity*>& selectedEntities = baseMapState.getSelectedEntities();

	int index = static_cast<int>(luaL_checkinteger(L, 2));
	if (index >= selectedEntities.size())
	{
		return 0;
	}

	lua_pushinteger(L, index + 1);
	entity::Entity* selectedEntity = *std::next(selectedEntities.begin(), index);
	FLAT_ASSERT(selectedEntity != nullptr);
	entity::lua::pushEntity(L, selectedEntity);
	return 2;
}
}

int l_Map_eachSelectedEntity(lua_State* L)
{
	lua_pushcfunction(L, locIterateOverSelectedEntities);
	lua_pushnil(L);
	lua_pushinteger(L, 0);
	return 3;
}


int l_Map_getEntitiesOfType(lua_State* L)
{
	const char* entityTemplateName = luaL_checkstring(L, 1);
	Game& game = flat::lua::getFlatAs<Game>(L);
	flat::state::State* state = game.getStateMachine().getState();
	states::BaseMapState& baseMapState = state->as<states::BaseMapState>();
	std::shared_ptr<const entity::EntityTemplate> entityTemplate = baseMapState.getEntityTemplate(game, entityTemplateName);
	lua_newtable(L);
	int i = 1;
	baseMapState.eachEntityOfType(entityTemplate, [L, &i](entity::Entity* entity)
	{
		entity::lua::pushEntity(L, entity);
		lua_rawseti(L, -2, i);
		++i;
	});
	return 1;
}

int l_Map_getZone(lua_State* L)
{
	const char* zoneName = luaL_checkstring(L, 1);
	const Map& map = getMap(L);
	std::shared_ptr<Zone> zone;
	if (!map.getZone(zoneName, zone))
	{
		luaL_error(L, "No zone named %s", zoneName);
	}
	zone::pushZone(L, zone);
	return 1;
}

int l_Map_getTilePosition(lua_State* L)
{
	TileIndex tileIndex = static_cast<TileIndex>(luaL_checkinteger(L, 1));
	const Map& map = getMap(L);
	const flat::Vector2i& position = map.getTileXY(tileIndex);
	float z = map.getTileZ(tileIndex);
	lua_pushinteger(L, position.x);
	lua_pushinteger(L, position.y);
	lua_pushnumber(L, z);
	return 3;
}

int l_Map_getTileZ(lua_State* L)
{
	TileIndex tileIndex = static_cast<TileIndex>(luaL_checkinteger(L, 1));
	const Map& map = getMap(L);
	lua_pushnumber(L, map.getTileZ(tileIndex));
	return 1;
}

int l_Map_setTileZ(lua_State* L)
{
	TileIndex tileIndex = static_cast<TileIndex>(luaL_checkinteger(L, 1));
	float z = static_cast<float>(luaL_checknumber(L, 2));
	Map& map = getMap(L);
	map.setTileZ(tileIndex, z);
	return 0;
}

int l_Map_moveTileZBy(lua_State* L)
{
	TileIndex tileIndex = static_cast<TileIndex>(luaL_checkinteger(L, 1));
	float dz = static_cast<float>(luaL_checknumber(L, 2));
	Map& map = getMap(L);
	map.moveTileZBy(tileIndex, dz);
	return 0;
}

game::map::Map& getMap(lua_State* L)
{
	Game& game = flat::lua::getFlatAs<Game>(L);
	flat::state::State* state = game.getStateMachine().getState();
	states::BaseMapState& mapState = state->as<states::BaseMapState>();
	return mapState.getMap();
}

} // map
} // lua
} // map
} // game