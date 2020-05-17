#ifndef GAME_ENTITY_FACTION_LUA_FACTION_H
#define GAME_ENTITY_FACTION_LUA_FACTION_H

#include <filesystem>

struct lua_State;

namespace game
{
namespace entity
{
namespace faction
{
namespace lua
{

int open(lua_State* L, const std::filesystem::path& factionsConfigPath);

int l_faction(lua_State* L);
int l_factionRelation(lua_State* L);

} // lua
} // faction
} // entity
} // game

#endif // GAME_ENTITY_FACTION_LUA_FACTION_H


