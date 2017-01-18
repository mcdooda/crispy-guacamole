#include "entity.h"
#include "../entity.h"
#include "../faction/faction.h"
#include "../component/components/attack/attackcomponent.h"
#include "../component/components/movement/movementcomponent.h"
#include "../component/components/sprite/spritecomponent.h"
#include "../component/components/detection/detectioncomponent.h"
#include "../component/components/faction/factioncomponent.h"
#include "../component/components/life/lifecomponent.h"
#include "../../game.h"
#include "../../states/basemapstate.h"

namespace game
{
namespace entity
{
using namespace component;
namespace lua
{

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	// Entity metatable
	luaL_newmetatable(L, "CG.Entity");
	// mt.__index = mt
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	
	static const luaL_Reg Entity_lib_m[] = {
		{"__eq",                    l_Entity_eq},

		{"isValid",                 l_Entity_isValid},

		{"getTemplateName",         l_Entity_getTemplateName},

		{"despawn",                 l_Entity_despawn},
		{"kill",                    l_Entity_kill},

		{"getExtraData",            l_Entity_getExtraData},

		{"setPosition",             l_Entity_setPosition},
		{"getPosition",             l_Entity_getPosition},

		{"setHeading",              l_Entity_setHeading},
		{"getHeading",              l_Entity_getHeading},

		{"setElevation",            l_Entity_setElevation},
		{"getElevation",            l_Entity_getElevation},

		{"lookAtEntity",            l_Entity_lookAtEntity},

		// movement
		{"moveTo",                  l_Entity_moveTo},
		{"clearPath",               l_Entity_clearPath},
		{"setSpeed",                l_Entity_setSpeed},
		{"getSpeed",                l_Entity_getSpeed},
		{"jump",                    l_Entity_jump},

		// behavior
		{"enterState",              l_Entity_enterState},
		{"enterStateAsync",         l_Entity_enterStateAsync},

		// sprite
		{"playAnimation",           l_Entity_playAnimation},
		{"playAnimationAsync",      l_Entity_playAnimationAsync},
		{"setMoveAnimation",        l_Entity_setMoveAnimation},
		{"setDefaultMoveAnimation", l_Entity_setDefaultMoveAnimation},
		{"getAttachPoint",          l_Entity_getAttachPoint},

		// detection
		{"canSee",                  l_Entity_canSee},

		// faction
		{"isNeutral",               l_Entity_isNeutral},
		{"isFriendly",              l_Entity_isFriendly},
		{"isHostile",               l_Entity_isHostile},

		// attack
		{"setAttackTarget",         l_Entity_setAttackTarget},
		{"getAttackTarget",         l_Entity_getAttackTarget},
		
		{nullptr, nullptr}
	};
	luaL_setfuncs(L, Entity_lib_m, 0);
	
	lua_createtable(L, 0, 1);
	static const luaL_Reg Entity_lib_f[] = {
		{"spawn", l_Entity_spawn},
		{nullptr, nullptr}
	};
	luaL_setfuncs(L, Entity_lib_f, 0);
	lua_setglobal(L, "Entity");

	lua_pop(L, 1);
	
	return 0;
}

int l_Entity_eq(lua_State* L)
{
	EntityHandle entityHandle1 = getEntityHandle(L, 1);
	EntityHandle entityHandle2 = getEntityHandle(L, 2);
	lua_pushboolean(L, entityHandle1 == entityHandle2);
	return 1;
}

int l_Entity_isValid(lua_State* L)
{
	EntityHandle entityHandle = getEntityHandle(L, 1);
	lua_pushboolean(L, entityHandle.isValid());
	return 1;
}

int l_Entity_getTemplateName(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	lua_pushstring(L, entity.getTemplateName().c_str());
	return 1;
}

int l_Entity_despawn(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	entity.markForDelete();
	return 0;
}

int l_Entity_kill(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	life::LifeComponent& lifeComponent = getComponent<life::LifeComponent>(L, entity);
	lifeComponent.kill();
	return 0;
}

int l_Entity_getExtraData(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	flat::lua::SharedLuaReference<LUA_TTABLE>& extraData = entity.getExtraData();
	if (!extraData)
	{
		lua_createtable(L, 0, 1);
		extraData.set(L, -1);
	}
	else
	{
		extraData.push(L);
	}
	return 1;
}

int l_Entity_setPosition(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	float x = static_cast<float>(luaL_checknumber(L, 2));
	float y = static_cast<float>(luaL_checknumber(L, 3));
	float z = static_cast<float>(luaL_checknumber(L, 4));
	flat::Vector3 position(x, y, z);
	entity.setPosition(position);
	return 0;
}

int l_Entity_getPosition(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	const flat::Vector3& position = entity.getPosition();
	lua_pushnumber(L, position.x);
	lua_pushnumber(L, position.y);
	lua_pushnumber(L, position.z);
	return 3;
}

int l_Entity_setHeading(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	float heading = static_cast<float>(luaL_checknumber(L, 2));
	entity.setHeading(heading);
	return 0;
}

int l_Entity_getHeading(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	lua_pushnumber(L, entity.getHeading());
	return 1;
}

int l_Entity_setElevation(lua_State * L)
{
	Entity& entity = getEntity(L, 1);
	float elevation = static_cast<float>(luaL_checknumber(L, 2));
	entity.setElevation(elevation);
	return 0;
}

int l_Entity_getElevation(lua_State * L)
{
	Entity& entity = getEntity(L, 1);
	lua_pushnumber(L, entity.getElevation());
	return 1;
}

int l_Entity_lookAtEntity(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	Entity& target = getEntity(L, 2);
	flat::Vector2 entityPosition2d(entity.getPosition());
	flat::Vector2 targetPosition2d(target.getPosition());
	float heading = flat::vector2_angle(targetPosition2d - entityPosition2d);
	entity.setHeading(heading);
	return 0;
}

int l_Entity_moveTo(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	float x = static_cast<float>(luaL_checknumber(L, 2));
	float y = static_cast<float>(luaL_checknumber(L, 3));
	movement::MovementComponent& movementComponent = getComponent<movement::MovementComponent>(L, entity);
	movementComponent.addPointOnPath(flat::Vector2(x, y));
	return lua_yield(L, 0);
}

int l_Entity_clearPath(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	movement::MovementComponent& movementComponent = getComponent<movement::MovementComponent>(L, entity);
	movementComponent.clearPath();
	return 0;
}

int l_Entity_setSpeed(lua_State * L)
{
	Entity& entity = getEntity(L, 1);
	float speed = static_cast<float>(luaL_checknumber(L, 2));
	movement::MovementComponent& movementComponent = getComponent<movement::MovementComponent>(L, entity);
	movementComponent.setSpeed(speed);
	return 0;
}

int l_Entity_getSpeed(lua_State * L)
{
	Entity& entity = getEntity(L, 1);
	movement::MovementComponent& movementComponent = getComponent<movement::MovementComponent>(L, entity);
	lua_pushnumber(L, movementComponent.getSpeed());
	return 1;
}

int l_Entity_jump(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	movement::MovementComponent& movementComponent = getComponent<movement::MovementComponent>(L, entity);
	if (!movementComponent.isTouchingGround())
	{
		luaL_error(L, "Cannot jump midair");
	}
	movementComponent.jump();
	return lua_yield(L, 0);
}

namespace
{
static void locEnterState(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	const char* stateName = luaL_checkstring(L, 2);
	entity.enterState(stateName);
}
}

int l_Entity_enterState(lua_State* L)
{
	locEnterState(L);
	return lua_yield(L, 0);
}

int l_Entity_enterStateAsync(lua_State * L)
{
	locEnterState(L);
	return 0;
}

namespace
{
static void locPlayAnimation(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	const char* animationName = luaL_checkstring(L, 2);
	int numLoops = luaL_optint(L, 3, 1);
	sprite::SpriteComponent& spriteComponent = getComponent<sprite::SpriteComponent>(L, entity);
	bool animationExists = spriteComponent.playAnimationByName(animationName, numLoops);
	if (!animationExists)
	{
		luaL_error(L, "%s has no %s animation", entity.getTemplateName().c_str(), animationName);
	}
}
}

int l_Entity_playAnimation(lua_State* L)
{
	locPlayAnimation(L);
	return lua_yield(L, 0);
}

int l_Entity_playAnimationAsync(lua_State * L)
{
	locPlayAnimation(L);
	return 0;
}

int l_Entity_setMoveAnimation(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	const char* moveAnimationName = luaL_checkstring(L, 2);
	sprite::SpriteComponent& spriteComponent = getComponent<sprite::SpriteComponent>(L, entity);
	bool animationExists = spriteComponent.setMoveAnimationByName(moveAnimationName);
	if (!animationExists)
	{
		luaL_error(L, "%s has no %s animation", entity.getTemplateName().c_str(), moveAnimationName);
	}
	return 0;
}

int l_Entity_setDefaultMoveAnimation(lua_State * L)
{
	Entity& entity = getEntity(L, 1);
	sprite::SpriteComponent& spriteComponent = getComponent<sprite::SpriteComponent>(L, entity);
	bool defaultMoveAnimationExists = spriteComponent.setDefaultMoveAnimation();
	if (!defaultMoveAnimationExists)
	{
		luaL_error(L, "%s has no default move animation", entity.getTemplateName().c_str());
	}
	return 0;
}

int l_Entity_getAttachPoint(lua_State * L)
{
	Entity& entity = getEntity(L, 1);
	const char* attachPointName = luaL_checkstring(L, 2);
	sprite::SpriteComponent& spriteComponent = getComponent<sprite::SpriteComponent>(L, entity);
	flat::Vector3 attachPoint;
	if (spriteComponent.getAttachPoint(attachPointName, attachPoint))
	{
		lua_pushnumber(L, attachPoint.x);
		lua_pushnumber(L, attachPoint.y);
		lua_pushnumber(L, attachPoint.z);
		return 3;
	}
	return 0;
}

int l_Entity_canSee(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	Entity& target = getEntity(L, 2);
	detection::DetectionComponent& detectionComponent = getComponent<detection::DetectionComponent>(L, entity);
	bool canSee = detectionComponent.isVisible(target);
	lua_pushboolean(L, canSee);
	return 1;
}

namespace
{
static entity::faction::FactionRelation locGetFactionRelation(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	Entity& other = getEntity(L, 2);

	component::faction::FactionComponent* factionComponent = entity.getComponent<component::faction::FactionComponent>();
	if (!factionComponent)
	{
		return entity::faction::FactionRelation::NEUTRAL;
	}

	component::faction::FactionComponent* otherFactionComponent = other.getComponent<component::faction::FactionComponent>();
	if (!otherFactionComponent)
	{
		return entity::faction::FactionRelation::NEUTRAL;
	}

	// other entity's relation towards the first entity
	return otherFactionComponent->getFaction().getFactionRelation(factionComponent->getFaction());
}
}

int l_Entity_isNeutral(lua_State* L)
{
	lua_pushboolean(L, locGetFactionRelation(L) == entity::faction::FactionRelation::NEUTRAL);
	return 1;
}

int l_Entity_isFriendly(lua_State* L)
{
	lua_pushboolean(L, locGetFactionRelation(L) == entity::faction::FactionRelation::FRIENDLY);
	return 1;
}

int l_Entity_isHostile(lua_State* L)
{
	lua_pushboolean(L, locGetFactionRelation(L) == entity::faction::FactionRelation::HOSTILE);
	return 1;
}

int l_Entity_setAttackTarget(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	Entity* target = getEntityPtr(L, 2);
	attack::AttackComponent& attackComponent = getComponent<attack::AttackComponent>(L, entity);
	attackComponent.setAttackTarget(target);
	return 0;
}

int l_Entity_getAttackTarget(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	attack::AttackComponent& attackComponent = getComponent<attack::AttackComponent>(L, entity);
	pushEntity(L, attackComponent.getAttackTarget());
	return 1;
}

// static lua functions
int l_Entity_spawn(lua_State* L)
{
	const char* entityTemplateName = luaL_checkstring(L, 1);
	float x = static_cast<float>(luaL_checknumber(L, 2));
	float y = static_cast<float>(luaL_checknumber(L, 3));
	float z = static_cast<float>(luaL_checknumber(L, 4));
	flat::Vector3 position(x, y, z);
	float heading = static_cast<float>(luaL_optnumber(L, 5, 0.f));
	float elevation = static_cast<float>(luaL_optnumber(L, 6, 0.f));
	Game& game = flat::lua::getGame(L).to<Game>();
	states::BaseMapState& baseMapState = game.getStateMachine().getState()->to<states::BaseMapState>();
	const std::shared_ptr<const EntityTemplate>& entityTemplate = baseMapState.getEntityTemplate(game, entityTemplateName);
	Entity* entity = baseMapState.spawnEntityAtPosition(game, entityTemplate, position, heading, elevation);
	pushEntity(L, entity);
	return 1;
}

EntityHandle getEntityHandle(lua_State * L, int index)
{
	return *static_cast<EntityHandle*>(luaL_checkudata(L, index, "CG.Entity"));
}

// private
Entity& getEntity(lua_State* L, int index)
{
	EntityHandle entityHandle = getEntityHandle(L, index);
	if (!entityHandle.isValid())
	{
		luaL_argerror(L, index, "The entity is not valid anymore");
	}
	return *entityHandle.getEntity();
}

Entity* getEntityPtr(lua_State* L, int index)
{
	if (lua_isnil(L, index))
	{
		return nullptr;
	}

	EntityHandle entityHandle = getEntityHandle(L, index);
	return entityHandle.getEntity();
}

void pushEntity(lua_State* L, Entity* entity)
{
	if (entity != nullptr)
	{
		EntityHandle* entityHandle = static_cast<EntityHandle*>(lua_newuserdata(L, sizeof(EntityHandle)));
		new (entityHandle) EntityHandle(entity);
		luaL_getmetatable(L, "CG.Entity");
		lua_setmetatable(L, -2);
	}
	else
	{
		lua_pushnil(L);
	}
}

} // lua
} // entity
} // game



