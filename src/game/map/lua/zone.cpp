#include "zone.h"
#include "../zone.h"
#include "../tile.h"
#include "../../entity/lua/entity.h"

namespace game
{
namespace map
{
namespace lua
{
namespace zone
{

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	// Zone metatable
	luaL_newmetatable(L, "CG.Zone");
	// mt.__index = mt
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	static const luaL_Reg Zone_lib_m[] = {
		{"__eq",             l_Zone_eq},

		{"getEntities",      l_Zone_getEntities},
		{"getEntitiesCount", l_Zone_getEntitiesCount},

		{"getCenter",        l_Zone_getCenter},

		{nullptr, nullptr}
	};

	luaL_setfuncs(L, Zone_lib_m, 0);

	lua_pop(L, 1);

	return 0;
}


int l_Zone_eq(lua_State * L)
{
	Zone* zone1 = getZone(L, 1);
	Zone* zone2 = getZone(L, 2);
	lua_pushboolean(L, zone1 == zone2);
	return 1;
}

int l_Zone_getEntities(lua_State * L)
{
	Zone* zone = getZone(L, 1);
	lua_newtable(L);
	int index = 1;
	zone->eachTileIfExists([L, &index](const Tile* tile)
	{
		for (entity::Entity* entity : tile->getEntities())
		{
			entity::lua::pushEntity(L, entity);
			lua_rawseti(L, -2, index);
			++index;
		}
	});
	return 1;
}

int l_Zone_getEntitiesCount(lua_State * L)
{
	Zone* zone = getZone(L, 1);
	lua_Integer count = 0;
	zone->eachTileIfExists([&count](const Tile* tile)
	{
		count += tile->getEntities().size();
	});
	lua_pushinteger(L, count);
	return 1;
}

int l_Zone_getCenter(lua_State * L)
{
	Zone* zone = getZone(L, 1);
	flat::Vector2 center = zone->getCenter();
	lua_pushnumber(L, center.x);
	lua_pushnumber(L, center.y);
	return 2;
}

Zone* getZone(lua_State* L, int index)
{
	return (*static_cast<std::weak_ptr<Zone>*>(luaL_checkudata(L, index, "CG.Zone"))).lock().get();
}

void pushZone(lua_State* L, const std::shared_ptr<Zone>& zone)
{
	if (zone != nullptr)
	{
		std::weak_ptr<Zone>* zoneWeakPtr = static_cast<std::weak_ptr<Zone>*>(lua_newuserdata(L, sizeof(std::weak_ptr<Zone>*)));
		new (zoneWeakPtr) std::weak_ptr<Zone>(zone);
		luaL_getmetatable(L, "CG.Zone");
		lua_setmetatable(L, -2);
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


