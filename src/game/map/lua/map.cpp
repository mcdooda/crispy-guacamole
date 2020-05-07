#include <memory>

#include "map.h"
#include "zone.h"

#include "game.h"

#include "map/tile.h"
#include "map/pathfinder/pathfinder.h"
#include "map/pathfinder/lua/path.h"
#include "map/fog/fog.h"

#include "states/mapeditorstate.h"
#include "entity/lua/entity.h"

namespace game::map::lua::map
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
		{"debug_enableSimplifyPath",      l_Map_debug_enableSimplifyPath},
#endif

		{"getNumEntities",                l_Map_getNumEntities},
		{"getEntitiesInRange",            l_Map_getEntitiesInRange},
		{"eachSelectedEntity",            l_Map_eachSelectedEntity},
		{"selectionChanged",              l_Map_selectionChanged},
		{"getEntitiesOfType",             l_Map_getEntitiesOfType},

		{"getZone",                       l_Map_getZone},

		{"getTilePosition",               l_Map_getTilePosition},
		{"getTileZ",                      l_Map_getTileZ},
		{"setTileZ",                      l_Map_setTileZ},
		{"moveTileZBy",                   l_Map_moveTileZBy},
		{"setTileTemplate",               l_Map_setTileTemplate},

		{"findPath",                      l_Map_findPath},

		{"setFogType",                    l_Map_setFogType},
		{"getFogType",                    l_Map_getFogType},

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

	lua_pushstring(L, "FogType");
	static const flat::lua::table::KeyValuePair<int> fogTypeTable[] = {
		{"NONE", fog::Fog::FogType::NONE},
		{"HARD", fog::Fog::FogType::HARD},
		{"SOFT", fog::Fog::FogType::SOFT},

		{nullptr, 0},
	};
	flat::lua::table::pushTable(L, fogTypeTable);
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
	std::string mapPath = luaL_checkstring(L, 1);
	Game& game = flat::lua::getFlatAs<Game>(L);
	game.mapPath = mapPath;
	states::BaseMapState& mapState = getMapState(L);
	bool mapLoaded = mapState.loadMap(game);
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

int l_Map_debug_enableSimplifyPath(lua_State* L)
{
	const bool enable = lua_toboolean(L, 1) == 1;
	pathfinder::Path::enableSimplifyPath(enable);
	return 0;
}
#endif // FLAT_DEBUG

int l_Map_getNumEntities(lua_State* L)
{
	const states::BaseMapState& baseMapState = getMapState(L);
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

	const states::BaseMapState& mapState = getMapState(L);
	const std::vector<entity::Entity*>& selectedEntities = mapState.getSelectedEntities();

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

int l_Map_selectionChanged(lua_State* L)
{
	states::BaseMapState& mapState = getMapState(L);
	mapState.addSelectionChangedCallback(L, 1);
	return 0;
}

int l_Map_getEntitiesOfType(lua_State* L)
{
	const char* entityTemplateName = luaL_checkstring(L, 1);
	Game& game = flat::lua::getFlatAs<Game>(L);
	states::BaseMapState& mapState = getMapState(L);
	std::shared_ptr<const entity::EntityTemplate> entityTemplate = mapState.getEntityTemplate(game, entityTemplateName);
	lua_newtable(L);
	int i = 1;
	mapState.eachEntityOfType(entityTemplate, [L, &i](entity::Entity* entity)
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
	const TileIndex tileIndex = static_cast<TileIndex>(luaL_checkinteger(L, 1));
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
	const TileIndex tileIndex = static_cast<TileIndex>(luaL_checkinteger(L, 1));
	const Map& map = getMap(L);
	lua_pushnumber(L, map.getTileZ(tileIndex));
	return 1;
}

int l_Map_setTileZ(lua_State* L)
{
	const TileIndex tileIndex = static_cast<TileIndex>(luaL_checkinteger(L, 1));
	const float z = static_cast<float>(luaL_checknumber(L, 2));
	Map& map = getMap(L);
	map.setTileZ(tileIndex, z);
	return 0;
}

int l_Map_moveTileZBy(lua_State* L)
{
	const TileIndex tileIndex = static_cast<TileIndex>(luaL_checkinteger(L, 1));
	const float dz = static_cast<float>(luaL_checknumber(L, 2));
	Map& map = getMap(L);
	map.moveTileZBy(tileIndex, dz);
	return 0;
}

int l_Map_setTileTemplate(lua_State* L)
{
	const TileIndex tileIndex = static_cast<TileIndex>(luaL_checkinteger(L, 1));
	const std::string tileTemplateName = luaL_checkstring(L, 2);
	Game& game = flat::lua::getFlatAs<Game>(L);
	std::shared_ptr<const game::map::TileTemplate> tileTemplate = getMapState(L).getTileTemplate(game, tileTemplateName);
	Map& map = getMap(L);
	map.setTileTemplate(tileIndex, tileTemplate);
	return 0;
}

int l_Map_findPath(lua_State* L)
{
	const flat::Vector2& from = flat::lua::getVector2(L, 1);
	const flat::Vector2& to = flat::lua::getVector2(L, 2);
	const bool allowPartialPath = lua_isnoneornil(L, 3) ? false : lua_toboolean(L, 3) == 1;

	const float jumpHeight = static_cast<Navigability>(luaL_checkinteger(L, 3));
	const Navigability navigability = static_cast<Navigability>(luaL_checkinteger(L, 4));
	const Map& map = getMap(L);
	pathfinder::Pathfinder pathfinder(map, jumpHeight, navigability);
	std::shared_ptr<pathfinder::Path> path = std::make_shared<pathfinder::Path>();

	pathfinder::Request request;
	request.from = from;
	request.to = to;
	request.allowPartialResult = allowPartialPath;

	pathfinder.findPath(request, *path);
	game::map::pathfinder::lua::pushPath(L, path);
	return 1;
}

int l_Map_setFogType(lua_State* L)
{
	const fog::Fog::FogType fogType = static_cast<fog::Fog::FogType>(luaL_checkinteger(L, 1));
	Map& map = getMap(L);
	map.setFogType(fogType);
	return 0;
}

int l_Map_getFogType(lua_State* L)
{
	const Map& map = getMap(L);
	lua_pushinteger(L, static_cast<lua_Integer>(map.getFogType()));
	return 1;
}

states::BaseMapState& getMapState(lua_State* L)
{
	Game& game = flat::lua::getFlatAs<Game>(L);
	flat::state::State* state = game.getStateMachine().getState();
	return state->as<states::BaseMapState>();
}

game::map::Map& getMap(lua_State* L)
{
	states::BaseMapState& mapState = getMapState(L);
	return mapState.getMap();
}

} // game::map::lua::map