#include "entitytemplate.h"
#include "behavior/behavior.h"
#include "../game.h"

namespace game
{
namespace entity
{

EntityTemplate::EntityTemplate(Game* game, lua_State* L, const std::string& path) :
	m_atlasWidth(1.f),
	m_atlasHeight(1.f),
	m_animationFrameDuration(1.f),
	m_radius(0.f)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	loadSpriteConfig(game, L, path);
	loadPhysicsConfig(game, L, path);
	loadBehaviorConfig(game, L, path);
}

EntityTemplate::~EntityTemplate()
{
	FLAT_DELETE(m_behavior);
}

void EntityTemplate::loadSpriteConfig(Game* game, lua_State* L, const std::string& path)
{
	std::string spriteConfigPath = path + "sprite.lua";
	luaL_loadfile(L, spriteConfigPath.c_str());
	lua_call(L, 0, 1);
	
	lua_getfield(L, -1, "origin");
	lua_rawgeti(L, -1, 1);
	m_spriteOrigin.x = luaL_checknumber(L, -1);
	
	lua_rawgeti(L, -2, 2);
	m_spriteOrigin.y = luaL_checknumber(L, -1);
	
	lua_pop(L, 3);
	
	lua_getfield(L, -1, "size");
	lua_rawgeti(L, -1, 1);
	m_atlasWidth = luaL_checkint(L, -1);
	
	lua_rawgeti(L, -2, 2);
	m_atlasHeight = luaL_checkint(L, -1);
	
	lua_pop(L, 3);
	
	lua_getfield(L, -1, "animationFrameDuration");
	m_animationFrameDuration = luaL_checknumber(L, -1);
	
	lua_pop(L, 2);
	
	m_atlas = game->video->getTexture(path + "atlas.png");
}

void EntityTemplate::loadPhysicsConfig(Game* game, lua_State* L, const std::string& path)
{
	std::string physicsConfigPath = path + "physics.lua";
	luaL_loadfile(L, physicsConfigPath.c_str());
	lua_call(L, 0, 1);
	
	lua_getfield(L, -1, "radius");
	m_radius = luaL_checknumber(L, -1);
	
	lua_getfield(L, -2, "speed");
	m_speed = luaL_checknumber(L, -1);
	
	lua_pop(L, 3);
}

void EntityTemplate::loadBehaviorConfig(Game* game, lua_State* L, const std::string& path)
{
	std::string behaviorConfigPath = path + "behavior.lua";
	m_behavior = new behavior::Behavior(L, behaviorConfigPath);
}

} // entity
} // game



