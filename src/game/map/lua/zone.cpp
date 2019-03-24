#include "zone.h"
#include "../zone.h"
#include "../map.h"
#include "../tile.h"
#include "../../entity/lua/entity.h"
#include "../../game.h"

namespace game
{
namespace map
{
namespace lua
{
namespace zone
{

using LuaZone = flat::lua::SharedCppValue<std::weak_ptr<Zone>>;

int open(Game& game)
{
	lua_State* L = game.lua->state;
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	static const luaL_Reg Zone_lib_m[] = {
		{"__eq",             l_Zone_eq},

		{"getEntities",      l_Zone_getEntities},
		{"getEntitiesCount", l_Zone_getEntitiesCount},

		{"getCenter",        l_Zone_getCenter},

		{"isTileInside",     l_Zone_isTileInside},
		{"getRandomTile",    l_Zone_getRandomTile},

		{nullptr, nullptr}
	};
	game.lua->registerClass<LuaZone>("CG.Zone", Zone_lib_m);

	return 0;
}


int l_Zone_eq(lua_State* L)
{
	Zone* zone1 = getZone(L, 1);
	Zone* zone2 = getZone(L, 2);
	lua_pushboolean(L, zone1 == zone2);
	return 1;
}

int l_Zone_getEntities(lua_State* L)
{
	Zone* zone = getZone(L, 1);
	lua_newtable(L);
	int index = 1;

	const Map& map = zone->getMap();
	zone->eachTile([L, &index, &map](TileIndex tileIndex)
	{
		map.eachTileEntity(
			tileIndex,
			[L, &index](entity::Entity* entity)
			{
				entity::lua::pushEntity(L, entity);
				lua_rawseti(L, -2, index);
				++index;
			}
		);
	});
	return 1;
}

int l_Zone_getEntitiesCount(lua_State* L)
{
	Zone* zone = getZone(L, 1);
	lua_Integer count = 0;
	const Map& map = zone->getMap();
	zone->eachTile([&count, &map](TileIndex tileIndex)
	{
		count += map.getTileEntityCount(tileIndex);
	});
	lua_pushinteger(L, count);
	return 1;
}

int l_Zone_getCenter(lua_State* L)
{
	Zone* zone = getZone(L, 1);
	flat::lua::pushVector2(L, zone->getCenter());
	return 1;
}

int l_Zone_isTileInside(lua_State* L)
{
	Zone* zone = getZone(L, 1);
	TileIndex tileIndex = static_cast<TileIndex>(luaL_checkinteger(L, 2));
	lua_pushboolean(L, zone->isTileInside(tileIndex));
	return 1;
}

int l_Zone_getRandomTile(lua_State* L)
{
	Zone* zone = getZone(L, 1);
	Game& game = flat::lua::getFlatAs<Game>(L);
	const Tile* tile = zone->getRandomTile(game);
	lua_pushlightuserdata(L, const_cast<Tile*>(tile));
	return 1;
}

Zone* getZone(lua_State* L, int index)
{
	return LuaZone::get(L, index).lock().get();
}

void pushZone(lua_State* L, const std::shared_ptr<Zone>& zone)
{
	if (zone != nullptr)
	{
		LuaZone::pushNew(L, zone);
	}
	else
	{
		lua_pushnil(L);
	}
}

} // zone
} // lua
} // map
} // game


