#include "entitytemplate.h"
#include "component/componentregistry.h"
//////////////
#include "component/components/behavior/behaviorcomponent.h"
#include "component/components/movement/movementcomponent.h"
#include "component/components/sprite/spritecomponent.h"
//////////////
#include "behavior/behavior.h"
#include "../game.h"

namespace game
{
namespace entity
{

EntityTemplate::EntityTemplate(Game& game, lua_State* L, const component::ComponentRegistry& componentRegistry, const std::string& path, const std::string& name) :
	m_name(name),
	m_radius(0.f),
	m_speed(0.f),
	m_jumpForce(0.f),
	m_weight(0.f),
	m_jumpMaxHeight(0.f),
	m_jumpDistance(0.f),
	m_componentFlags(0)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	loadBehaviorConfig(L, path);
	loadMovementConfig(L, path);
	loadSpriteConfig(game, L, path);
	m_componentTemplates.reserve(componentRegistry.getNumComponentTypes());
	componentRegistry.eachComponentType([this, &game, L, &path](const component::ComponentType& componentType)
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		std::string configPath = path + componentType.getConfigName() + ".lua";
		int code = luaL_loadfile(L, configPath.c_str());
		if (code == LUA_OK)
		{
			lua_call(L, 0, 1);
			std::shared_ptr<component::ComponentTemplate> componentTemplate = componentType.loadConfigFile(game, L, path);
			if (componentTemplate)
			{
				m_componentTemplates.push_back(componentTemplate);
				m_componentFlags |= componentType.getComponentTypeFlag();
			}
			lua_pop(L, 1); // pop config table
		}
		else if (code == LUA_ERRFILE)
		{
			// LUA_ERRFILE -> component does not exist -> not an error -> pop error message and continue
			lua_pop(L, 1);
		}
		else
		{
			luaL_error(L, "luaL_loadfile failed with error %s", flat::lua::codeToString(code));
		}
	});
}

EntityTemplate::~EntityTemplate()
{
	FLAT_DELETE(m_behavior);
}

void EntityTemplate::loadSpriteConfig(Game& game, lua_State* L, const std::string& path)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	
	std::shared_ptr<const flat::video::Texture> atlas = game.video->getTexture(path + "atlas.png");
	m_spriteDescription.setAtlas(atlas);
	
	std::string spriteConfigPath = path + "sprite.lua";
	luaL_loadfile(L, spriteConfigPath.c_str());
	lua_call(L, 0, 1);
	
	// atlas size
	lua_getfield(L, -1, "size");
	lua_rawgeti(L, -1, 1);
	int atlasWidth = luaL_checkint(L, -1);
	m_spriteDescription.setAtlasWidth(atlasWidth);

	lua_rawgeti(L, -2, 2);
	int atlasHeight = luaL_checkint(L, -1);
	m_spriteDescription.setAtlasHeight(atlasHeight);

	lua_pop(L, 3);
	
	// sprite origin
	flat::Vector2 origin;
	lua_getfield(L, -1, "origin");
	lua_rawgeti(L, -1, 1);
	origin.x = static_cast<float>(luaL_checknumber(L, -1));
	lua_rawgeti(L, -2, 2);
	origin.y = static_cast<float>(luaL_checknumber(L, -1));
	m_spriteDescription.setOrigin(origin);

	lua_pop(L, 3);
	
	// animations
	lua_getfield(L, -1, "animations");
	luaL_checktype(L, -1, LUA_TTABLE);
	
	lua_pushnil(L);
	while (lua_next(L, -2) != 0)
	{
		const char* animationName = luaL_checkstring(L, -2);
		
		luaL_checktype(L, -1, LUA_TTABLE);
		lua_getfield(L, -1, "line");
		int line = luaL_checkint(L, -1);
		if (line > atlasHeight)
		{
			luaL_error(L, "Animation %s, line = %d is out of the atlas", animationName, line);
		}
		lua_getfield(L, -2, "numFrames");
		int numFrames = luaL_checkint(L, -1);
		if (numFrames > atlasWidth)
		{
			luaL_error(L, "Animation %s, numFrames = %d is out of the atlas", animationName, numFrames);
		}
		lua_getfield(L, -3, "frameDuration");
		float frameDuration = static_cast<float>(luaL_checknumber(L, -1));
		
		sprite::AnimationDescription animationDescription(line - 1, numFrames, frameDuration);
		m_spriteDescription.addAnimationDescription(animationName, animationDescription);
		
		lua_pop(L, 4);
	}
	m_spriteDescription.cacheMandatoryAnimationDescriptions();
	
	lua_pop(L, 2);

	if (m_spriteDescription.getAtlas())
		m_componentFlags |= component::SpriteComponent::getFlag();
}

void EntityTemplate::loadMovementConfig(lua_State* L, const std::string& path)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	
	std::string physicsConfigPath = path + "movement.lua";
	luaL_loadfile(L, physicsConfigPath.c_str());
	lua_call(L, 0, 1);
	
	lua_getfield(L, -1, "radius");
	m_radius = static_cast<float>(luaL_checknumber(L, -1));
	
	lua_getfield(L, -2, "speed");
	m_speed = static_cast<float>(luaL_checknumber(L, -1));
	
	lua_getfield(L, -3, "jumpForce");
	m_jumpForce = static_cast<float>(luaL_checknumber(L, -1));
	
	lua_getfield(L, -4, "weight");
	m_weight = static_cast<float>(luaL_checknumber(L, -1));
	
	// compute jump height and distance from jump force and weight
	//const float a = -m_weight / 2.f;
	//const float b = m_jumpForce;
	//const float delta = b * b;
	//m_jumpDuration = (-b - std::sqrt(delta)) / (2.f * a);
	m_jumpDuration = (2.f * m_jumpForce) / m_weight;
	m_jumpMaxHeight = getJumpHeight(m_jumpDuration / 2.f);
	m_jumpDistance = m_speed * m_jumpDuration;
	
	lua_pop(L, 5);

	if (m_speed > 0.f)
		m_componentFlags |= component::MovementComponent::getFlag();
}

void EntityTemplate::loadBehaviorConfig(lua_State* L, const std::string& path)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	
	std::string behaviorConfigPath = path + "behavior.lua";

	luaL_loadfile(L, behaviorConfigPath.c_str());
	lua_call(L, 0, 1);
	m_behavior = new behavior::Behavior(L);
	lua_pop(L, 1);

	m_componentFlags |= component::BehaviorComponent::getFlag();
}

} // entity
} // game



