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

using LuaZone = flat::lua::SharedCppValue<std::weak_ptr<Zone>>;

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	static const luaL_Reg Zone_lib_m[] = {
		{"__eq",             l_Zone_eq},

		{"getEntities",      l_Zone_getEntities},
		{"getEntitiesCount", l_Zone_getEntitiesCount},

		{"getCenter",        l_Zone_getCenter},

		{nullptr, nullptr}
	};
	LuaZone::registerClass("CG.Zone", L, Zone_lib_m);

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


