#include <iterator>
#include "entity.h"
#include "../entity.h"
#include "../faction/faction.h"
#include "../component/components/attack/attackcomponent.h"
#include "../component/components/behavior/behaviorcomponent.h"
#include "../component/components/detection/detectioncomponent.h"
#include "../component/components/faction/factioncomponent.h"
#include "../component/components/interaction/interactioncomponent.h"
#include "../component/components/life/lifecomponent.h"
#include "../component/components/movement/movementcomponent.h"
#include "../component/components/playercontroller/playercontrollercomponent.h"
#include "../component/components/projectile/projectilecomponent.h"
#include "../component/components/selection/selectioncomponent.h"
#include "../component/components/sprite/spritecomponent.h"
#include "../component/components/ui/uicomponent.h"
#include "../../states/basemapstate.h"
#include "../../game.h"
#include "../../map/map.h"
#include "../../map/tile.h"

namespace game
{
namespace entity
{
using namespace component;
namespace lua
{

using LuaEntityHandle = flat::lua::SharedCppValue<EntityHandle>;

int open(Game& game)
{
	lua_State* L = game.lua->state;
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	
	static const luaL_Reg Entity_lib_m[] = {
		{"__eq",                     l_Entity_eq},

		{"isValid",                  l_Entity_isValid},
		{"delete",                   l_Entity_delete},

		{"getTemplateName",          l_Entity_getTemplateName},
		{"hasComponent",             l_Entity_hasComponent},
		{"decComponentDisableLevel", l_Entity_decComponentDisableLevel},
		{"incComponentDisableLevel", l_Entity_incComponentDisableLevel},
		{"isComponentEnabled",       l_Entity_isComponentEnabled},

#ifdef FLAT_DEBUG
		{"setDebug",                 l_Entity_setDebug},
		{"getDebug",                 l_Entity_getDebug},
		{"setComponentDebug",        l_Entity_setComponentDebug},
		{"getComponentDebug",        l_Entity_getComponentDebug},
#endif

		{"despawn",                  l_Entity_despawn},

		{"getExtraData",             l_Entity_getExtraData},

		{"setPosition",              l_Entity_setPosition},
		{"getPosition",              l_Entity_getPosition},

		{"setHeading",               l_Entity_setHeading},
		{"getHeading",               l_Entity_getHeading},
		{"headingChanged",           l_Entity_headingChanged},

		{"setElevation",             l_Entity_setElevation},
		{"getElevation",             l_Entity_getElevation},

		{"lookAtEntity",             l_Entity_lookAtEntity},

		// ui
		{"setUiOffset",              l_Entity_setUiOffset},
		{"setUiVisible",             l_Entity_setUiVisible},

		// movement
		{"moveTo",                   l_Entity_moveTo},
		{"clearPath",                l_Entity_clearPath},
		{"setSpeed",                 l_Entity_setSpeed},
		{"getSpeed",                 l_Entity_getSpeed},
		{"jump",                     l_Entity_jump},
		{"isTouchingGround",         l_Entity_isTouchingGround},
		{"restrictToZone",           l_Entity_restrictToZone},
		{"setMoveAnimation",         l_Entity_setMoveAnimation},
		{"setDefaultMoveAnimation",  l_Entity_setDefaultMoveAnimation},
		{"setIsStrafing",            l_Entity_setIsStrafing},
		{"getIsStrafing",            l_Entity_getIsStrafing},

		// behavior
		{"enterState",               l_Entity_enterState},
		{"sleep",                    l_Entity_sleep},
		{"getInteractionEntity",     l_Entity_getInteractionEntity},
		{"getInteractionStateName",  l_Entity_getInteractionStateName},
		{"interactWith",             l_Entity_interactWith},

		// sprite
		{"playAnimation",            l_Entity_playAnimation},
		{"getAttachPoint",           l_Entity_getAttachPoint},
		{"flipSpriteX",              l_Entity_flipSpriteX},
		{"setSpriteRotation",        l_Entity_setSpriteRotation},

		// detection
		{"canSee",                   l_Entity_canSee},
		{"eachVisibleEntity",        l_Entity_eachVisibleEntity},

		// faction
		{"isNeutral",                l_Entity_isNeutral},
		{"isFriendly",               l_Entity_isFriendly},
		{"isHostile",                l_Entity_isHostile},

		// attack
		{"setAttackTarget",          l_Entity_setAttackTarget},
		{"getAttackTarget",          l_Entity_getAttackTarget},
		{"isInAttackRange",          l_Entity_isInAttackRange},

		// life
		{"isLiving",                 l_Entity_isLiving},
		{"kill",                     l_Entity_kill},
		{"dealDamage",               l_Entity_dealDamage},
		{"getHealth",                l_Entity_getHealth},
		{"getMaxHealth",             l_Entity_getMaxHealth},
		{"healthChanged",            l_Entity_healthChanged},
		{"died",                     l_Entity_died},

		// selection
		{"selected",                 l_Entity_selected},
		{"deselected",               l_Entity_deselected},
		{"click",                    l_Entity_click},

		// projectile
		{"setProjectileSpeed",       l_Entity_setProjectileSpeed},
		{"getProjectileSpeed",       l_Entity_getProjectileSpeed},

		// player controller
		{"setGamepadIndex",          l_Entity_setGamepadIndex},
		{"getGamepadIndex",          l_Entity_getGamepadIndex},
		
		{nullptr, nullptr}
	};
	game.lua->registerClass<LuaEntityHandle>("CG.Entity", Entity_lib_m);
	
	lua_createtable(L, 0, 3);
	static const luaL_Reg Entity_lib_f[] = {
		{"spawn",              l_Entity_spawn},

		{"setGhostTemplate",   l_Entity_setGhostTemplate},
		{"clearGhostTemplate", l_Entity_clearGhostTemplate},

		{nullptr, nullptr}
	};
	luaL_setfuncs(L, Entity_lib_f, 0);
	lua_setglobal(L, "Entity");
	
	return 0;
}

namespace
{
static int locYieldIf(lua_State* L, bool yield, int numReturnValues)
{
	return yield ? lua_yield(L, numReturnValues) : numReturnValues;
}

static bool locGetOptBool(lua_State* L, int index, bool default)
{
	return lua_isnone(L, index) ? true : lua_toboolean(L, index) == 1;
}
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

int l_Entity_delete(lua_State * L)
{
	Entity& entity = getEntity(L, 1);
	entity.markForDelete();
	return 0;
}

int l_Entity_getTemplateName(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	lua_pushstring(L, entity.getTemplateName().c_str());
	return 1;
}

int l_Entity_hasComponent(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	component::ComponentFlags componentFlag = static_cast<component::ComponentFlags>(luaL_checkinteger(L, 2));
	luaL_argcheck(L, componentFlag != 0, 2, "componentFlag must not be zero");
	component::Component* component = entity.findComponent(componentFlag);
	lua_pushboolean(L, component != nullptr);
	return 1;
}

int l_Entity_decComponentDisableLevel(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	component::ComponentFlags componentFlag = static_cast<component::ComponentFlags>(luaL_checkinteger(L, 2));
	luaL_argcheck(L, componentFlag != 0, 2, "componentFlag must not be zero");
	component::Component* component = entity.findComponent(componentFlag);
	luaL_argcheck(L, component != nullptr, 2, "entity does not have this component");
	component->decDisableLevel();
	return 0;
}

int l_Entity_incComponentDisableLevel(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	component::ComponentFlags componentFlag = static_cast<component::ComponentFlags>(luaL_checkinteger(L, 2));
	luaL_argcheck(L, componentFlag != 0, 2, "componentFlag must not be zero");
	component::Component* component = entity.findComponent(componentFlag);
	luaL_argcheck(L, component != nullptr, 2, "entity does not have this component");
	component->incDisableLevel();
	return 0;
}

int l_Entity_isComponentEnabled(lua_State * L)
{
	Entity& entity = getEntity(L, 1);
	component::ComponentFlags componentFlag = static_cast<component::ComponentFlags>(luaL_checkinteger(L, 2));
	luaL_argcheck(L, componentFlag != 0, 2, "componentFlag must not be zero");
	component::Component* component = entity.findComponent(componentFlag);
	luaL_argcheck(L, component != nullptr, 2, "entity does not have this component");
	lua_pushboolean(L, component->isEnabled());
	return 1;
}

#ifdef FLAT_DEBUG
int l_Entity_setDebug(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	bool debug = lua_toboolean(L, 2) == 1;
	entity.setDebug(debug);
	return 0;
}

int l_Entity_getDebug(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	lua_pushboolean(L, entity.getDebug());
	return 1;
}

int l_Entity_setComponentDebug(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	component::ComponentFlags componentFlag = static_cast<component::ComponentFlags>(luaL_checkinteger(L, 2));
	bool debug = lua_toboolean(L, 3) == 1;
	component::Component* component = entity.findComponent(componentFlag);
	luaL_argcheck(L, component != nullptr, 2, "component does not exist");
	component->setDebug(debug);
	return 0;
}

int l_Entity_getComponentDebug(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	component::ComponentFlags componentFlag = static_cast<component::ComponentFlags>(luaL_checkinteger(L, 2));
	component::Component* component = entity.findComponent(componentFlag);
	luaL_argcheck(L, component != nullptr, 2, "component does not exist");
	lua_pushboolean(L, component->getDebug());
	return 1;
}
#endif // FLAT_DEBUG

int l_Entity_despawn(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	entity.markForDelete();
	return 0;
}

int l_Entity_getExtraData(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	const bool keyGiven = lua_isstring(L, 2);

	flat::lua::UniqueLuaReference<LUA_TTABLE>& extraData = entity.getExtraData();
	if (!extraData)
	{
		lua_createtable(L, 0, 1);
		extraData.set(L, -1);

		if (keyGiven)
		{
			lua_createtable(L, 0, 1);
			lua_pushvalue(L, 2); // push key
			lua_pushvalue(L, -2); // just created value
			lua_rawset(L, -4);
			lua_pushboolean(L, true);
			return 2;
		}
	}
	else
	{
		extraData.push(L);

		if (keyGiven)
		{
			lua_pushvalue(L, 2);
			lua_rawget(L, -2);
			if (lua_isnil(L, -1))
			{
				lua_createtable(L, 0, 1);
				lua_pushvalue(L, 2); // key already on the stack
				lua_pushvalue(L, -2); // just created value
				lua_rawset(L, -5);
				lua_pushboolean(L, true);
			}
			else
			{
				lua_pushboolean(L, false);
			}
			
			return 2;
		}
	}

	return 1;
}

int l_Entity_setPosition(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	flat::Vector3 position = flat::lua::getVector3(L, 2);
	entity.setPosition(position);
	return 0;
}

int l_Entity_getPosition(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	flat::lua::pushVector3(L, entity.getPosition());
	return 1;
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

int l_Entity_headingChanged(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	flat::lua::SharedLuaReference<LUA_TFUNCTION> onHeadingChanged(L, 2);
	entity.headingChanged.on([L, &entity, onHeadingChanged](float heading)
	{
		bool keepCallback = true;
		onHeadingChanged.callFunction(
			[&entity, heading](lua_State* L)
			{
				entity::lua::pushEntity(L, &entity);
				lua_pushnumber(L, heading);
			},
			1,
			[&keepCallback](lua_State* L)
			{
				keepCallback = lua_toboolean(L, -1) == 1;
			}
		);
		return keepCallback;
	});
	return 0;
}

int l_Entity_setElevation(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	float elevation = static_cast<float>(luaL_checknumber(L, 2));
	entity.setElevation(elevation);
	return 0;
}

int l_Entity_getElevation(lua_State* L)
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

int l_Entity_setUiOffset(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	flat::Vector2 offset = flat::lua::getVector2(L, 2);
	ui::UiComponent& uiComponent = getComponent<ui::UiComponent>(L, entity);
	uiComponent.setWidgetOffset(offset);
	return 0;
}

int l_Entity_setUiVisible(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	bool visible = lua_toboolean(L, 2) == 1;
	ui::UiComponent& uiComponent = getComponent<ui::UiComponent>(L, entity);
	uiComponent.setWidgetVisible(visible);
	return 0;
}

// MOVEMENT

int l_Entity_moveTo(lua_State* L)
{
	FLAT_PROFILE("entity:moveTo()");

	Entity& entity = getEntity(L, 1);
	flat::Vector2 pathPoint = flat::lua::getVector2(L, 2);
	bool yield = locGetOptBool(L, 3, true);
	movement::MovementComponent& movementComponent = getComponent<movement::MovementComponent>(L, entity);
	movementComponent.moveTo(pathPoint);
	return locYieldIf(L, yield, 0);
}

int l_Entity_clearPath(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	movement::MovementComponent& movementComponent = getComponent<movement::MovementComponent>(L, entity);
	movementComponent.clearPath();
	return 0;
}

int l_Entity_setSpeed(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	float speed = static_cast<float>(luaL_checknumber(L, 2));
	movement::MovementComponent& movementComponent = getComponent<movement::MovementComponent>(L, entity);
	movementComponent.setSpeed(speed);
	return 0;
}

int l_Entity_getSpeed(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	movement::MovementComponent& movementComponent = getComponent<movement::MovementComponent>(L, entity);
	lua_pushnumber(L, movementComponent.getSpeed());
	return 1;
}

int l_Entity_jump(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	bool yield = locGetOptBool(L, 2, true);
	movement::MovementComponent& movementComponent = getComponent<movement::MovementComponent>(L, entity);
	movementComponent.jump();
	return locYieldIf(L, yield, 0);
}

int l_Entity_isTouchingGround(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	movement::MovementComponent& movementComponent = getComponent<movement::MovementComponent>(L, entity);
	lua_pushboolean(L, movementComponent.isTouchingGround());
	return 1;
}

int l_Entity_restrictToZone(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	movement::MovementComponent& movementComponent = getComponent<movement::MovementComponent>(L, entity);
	const char* zoneName = luaL_checkstring(L, 2);
	Game& game = flat::lua::getFlatAs<Game>(L);
	states::BaseMapState& baseMapState = game.getStateMachine().getState()->to<states::BaseMapState>();
	map::Map& map = baseMapState.getMap();
	std::shared_ptr<map::Zone> zone;
	if (!map.getZone(zoneName, zone))
	{
		luaL_error(L, "No zone named %s", zoneName);
	}
	movementComponent.restrictToZone(zone);
	return 0;
}

int l_Entity_setMoveAnimation(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	const char* moveAnimationName = luaL_checkstring(L, 2);
	movement::MovementComponent& movementComponent = getComponent<movement::MovementComponent>(L, entity);
	bool animationExists = movementComponent.setMoveAnimationByName(moveAnimationName);
	if (!animationExists)
	{
		luaL_error(L, "%s has no %s animation", entity.getTemplateName().c_str(), moveAnimationName);
	}
	return 0;
}

int l_Entity_setDefaultMoveAnimation(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	movement::MovementComponent& movementComponent = getComponent<movement::MovementComponent>(L, entity);
	bool defaultMoveAnimationExists = movementComponent.setDefaultMoveAnimation();
	if (!defaultMoveAnimationExists)
	{
		luaL_error(L, "%s has no default move animation", entity.getTemplateName().c_str());
	}
	return 0;
}

int l_Entity_setIsStrafing(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	bool isStrafing = lua_toboolean(L, 2) == 1;
	movement::MovementComponent& movementComponent = getComponent<movement::MovementComponent>(L, entity);
	movementComponent.setIsStrafing(isStrafing);
	return 0;
}

int l_Entity_getIsStrafing(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	movement::MovementComponent& movementComponent = getComponent<movement::MovementComponent>(L, entity);
	bool isStrafing = movementComponent.getIsStrafing();
	lua_pushboolean(L, isStrafing);
	return 1;
}

// BEHAVIOR

int l_Entity_enterState(lua_State* L)
{
	FLAT_PROFILE("entity:enterState()");

	Entity& entity = getEntity(L, 1);
	const char* stateName = luaL_checkstring(L, 2);
	bool yield = locGetOptBool(L, 3, true);
	entity.enterState(stateName);
	return locYieldIf(L, yield, 0);
}

int l_Entity_sleep(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	float duration = static_cast<float>(luaL_checknumber(L, 2));

	Game& game = flat::lua::getFlatAs<Game>(L);
	states::BaseMapState& baseMapState = game.getStateMachine().getState()->to<states::BaseMapState>();
	const flat::time::Clock& clock = baseMapState.getGameClock();
	const float time = clock.getTime();

	behavior::BehaviorComponent& behaviorComponent = getComponent<behavior::BehaviorComponent>(L, entity);
	behaviorComponent.sleep(time, duration);

	return lua_yield(L, 0);
}

int l_Entity_getInteractionEntity(lua_State * L)
{
	Entity& entity = getEntity(L, 1);
	behavior::BehaviorComponent& behaviorComponent = getComponent<behavior::BehaviorComponent>(L, entity);
	Entity* interactionEntity = behaviorComponent.getInteractionEntity().getEntity();
	pushEntity(L, interactionEntity);
	return 1;
}

int l_Entity_getInteractionStateName(lua_State * L)
{
	Entity& entity = getEntity(L, 1);
	behavior::BehaviorComponent& behaviorComponent = getComponent<behavior::BehaviorComponent>(L, entity);
	lua_pushstring(L, behaviorComponent.getInteractionStateName());
	return 1;
}

int l_Entity_interactWith(lua_State * L)
{
	Entity& entity = getEntity(L, 1);
	Entity& interactionEntity = getEntity(L, 2);

	const interaction::InteractionComponent& interactionComponent = getComponent<interaction::InteractionComponent>(L, interactionEntity);
	const char* stateName = interactionComponent.getBehaviorStateName().data();

	behavior::BehaviorComponent& behaviorComponent = getComponent<behavior::BehaviorComponent>(L, entity);
	behaviorComponent.setInteractionIfCompatible(stateName, &interactionEntity);

	entity.moveTo(flat::Vector2(interactionEntity.getPosition()), &interactionEntity);

	return 0;
}

// SPRITE

int l_Entity_playAnimation(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	const char* animationName = luaL_checkstring(L, 2);
	int numLoops = static_cast<int>(luaL_optinteger(L, 3, 1));
	bool yield = locGetOptBool(L, 4, true);
	sprite::SpriteComponent& spriteComponent = getComponent<sprite::SpriteComponent>(L, entity);
	bool animationExists = spriteComponent.playAnimationByName(animationName, numLoops, !yield);
	if (!animationExists)
	{
		luaL_error(L, "%s has no %s animation", entity.getTemplateName().c_str(), animationName);
	}
	return locYieldIf(L, yield, 0);
}

int l_Entity_getAttachPoint(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	const char* attachPointName = luaL_checkstring(L, 2);
	sprite::SpriteComponent& spriteComponent = getComponent<sprite::SpriteComponent>(L, entity);
	flat::Vector3 attachPoint;
	if (spriteComponent.getAttachPoint(attachPointName, attachPoint))
	{
		flat::lua::pushVector3(L, attachPoint);
		return 1;
	}
	return 0;
}


int l_Entity_flipSpriteX(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	bool flip = lua_toboolean(L, 2) == 1;

	// check the component is present
	getComponent<sprite::SpriteComponent>(L, entity);

	entity.getSprite().setFlipX(flip);
	return 0;
}

int l_Entity_setSpriteRotation(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	float angle = static_cast<float>(luaL_checknumber(L, 2));

	// check the component is present
	getComponent<sprite::SpriteComponent>(L, entity);

	entity.getSprite().setRotationZ(angle);
	entity.setAABBDirty();
	return 0;
}

// DETECTION

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
static int locIterateOverVisibleEntities(lua_State*L)
{
	Entity& entity = getEntity(L, 1);
	detection::DetectionComponent& detectionComponent = getComponent<detection::DetectionComponent>(L, entity);
	const std::set<Entity*>& visibleEntities = detectionComponent.getVisibleEntities();

	int index = static_cast<int>(luaL_checkinteger(L, 2));
	if (index >= visibleEntities.size())
	{
		return 0;
	}

	lua_pushinteger(L, index + 1);
	Entity* visibleEntity = *std::next(visibleEntities.begin(), index);
	FLAT_ASSERT(visibleEntity != nullptr);
	pushEntity(L, visibleEntity);
	return 2;
}
}

int l_Entity_eachVisibleEntity(lua_State* L)
{
	luaL_checkudata(L, 1, "CG.Entity");
	lua_pushcfunction(L, locIterateOverVisibleEntities);
	lua_pushvalue(L, 1);
	lua_pushinteger(L, 0);
	return 3;
}

// FACTION

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

// ATTACK

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

int l_Entity_isInAttackRange(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	Entity& target = getEntity(L, 2);
	attack::AttackComponent& attackComponent = getComponent<attack::AttackComponent>(L, entity);
	lua_pushboolean(L, attackComponent.isInAttackRange(&target));
	return 1;
}

// LIFE

int l_Entity_isLiving(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	life::LifeComponent* lifeComponent = entity.getComponent<life::LifeComponent>();
	lua_pushboolean(L, lifeComponent != nullptr);
	return 1;
}

int l_Entity_kill(lua_State* L)
{
	FLAT_PROFILE("entity:kill()");

	Entity& entity = getEntity(L, 1);
	life::LifeComponent& lifeComponent = getComponent<life::LifeComponent>(L, entity);
	lifeComponent.kill();
	return 0;
}

int l_Entity_dealDamage(lua_State* L)
{
	FLAT_PROFILE("entity:dealDamage()");

	Entity& entity = getEntity(L, 1);
	int damage = static_cast<int>(luaL_checkinteger(L, 2));
	life::LifeComponent& lifeComponent = getComponent<life::LifeComponent>(L, entity);
	lifeComponent.dealDamage(damage);
	return 0;
}

int l_Entity_getHealth(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	life::LifeComponent& lifeComponent = getComponent<life::LifeComponent>(L, entity);
	lua_pushinteger(L, lifeComponent.getHealth());
	return 1;
}

int l_Entity_getMaxHealth(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	life::LifeComponent& lifeComponent = getComponent<life::LifeComponent>(L, entity);
	lua_pushinteger(L, lifeComponent.getMaxHealth());
	return 1;
}

int l_Entity_healthChanged(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	life::LifeComponent& lifeComponent = getComponent<life::LifeComponent>(L, entity);
	lifeComponent.addHealthChangedCallback(L, 2);
	return 0;
}

int l_Entity_died(lua_State * L)
{
	Entity& entity = getEntity(L, 1);
	life::LifeComponent& lifeComponent = getComponent<life::LifeComponent>(L, entity);
	lifeComponent.addDiedCallback(L, 2);
	return 0;
}

// SELECTION

int l_Entity_selected(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	selection::SelectionComponent& selectionComponent = getComponent<selection::SelectionComponent>(L, entity);
	selectionComponent.addSelectedCallback(L, 2);
	return 0;
}

int l_Entity_deselected(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	selection::SelectionComponent& selectionComponent = getComponent<selection::SelectionComponent>(L, entity);
	selectionComponent.addDeselectedCallback(L, 2);
	return 0;
}

int l_Entity_click(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	selection::SelectionComponent& selectionComponent = getComponent<selection::SelectionComponent>(L, entity);
	selectionComponent.addClickCallback(L, 2);
	return 0;
}

// PROJECTILE

int l_Entity_setProjectileSpeed(lua_State * L)
{
	Entity& entity = getEntity(L, 1);
	flat::Vector3& speed = flat::lua::getVector3(L, 2);
	projectile::ProjectileComponent& projectileComponent = getComponent<projectile::ProjectileComponent>(L, entity);
	projectileComponent.setSpeed(speed);
	return 0;
}

int l_Entity_getProjectileSpeed(lua_State * L)
{
	Entity& entity = getEntity(L, 1);
	projectile::ProjectileComponent& projectileComponent = getComponent<projectile::ProjectileComponent>(L, entity);
	flat::lua::pushVector3(L, projectileComponent.getSpeed());
	return 1;
}

int l_Entity_setGamepadIndex(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	flat::input::GamepadIndex gamepadIndex = static_cast<flat::input::GamepadIndex>(luaL_checkinteger(L, 2));
	luaL_argcheck(L, gamepadIndex >= 1, 2, "invalid gamepad index");
	playercontroller::PlayerControllerComponent& playerControllerComponent = getComponent<playercontroller::PlayerControllerComponent>(L, entity);
	playerControllerComponent.setGamepadIndex(gamepadIndex - 1);
	return 0;
}

int l_Entity_getGamepadIndex(lua_State* L)
{
	Entity& entity = getEntity(L, 1);
	playercontroller::PlayerControllerComponent& playerControllerComponent = getComponent<playercontroller::PlayerControllerComponent>(L, entity);
	lua_pushinteger(L, playerControllerComponent.getGamepadIndex() + 1);
	return 1;
}

// static lua functions
int l_Entity_spawn(lua_State* L)
{
	// entity template
	const char* entityTemplateName = luaL_checkstring(L, 1);

	Game& game = flat::lua::getFlatAs<Game>(L);
	states::BaseMapState& baseMapState = game.getStateMachine().getState()->to<states::BaseMapState>();

	// position (Vector2 or Vector3)
	flat::Vector3 position;
	if (luaL_testudata(L, 2, "flat.Vector3") != nullptr) // TODO: add flat::lua::isVector2/3()
	{
		position = flat::lua::getVector3(L, 2);
	}
	else
	{
		position = flat::Vector3(flat::lua::getVector2(L, 2), 0.f);
		const map::TileIndex tileIndex = baseMapState.getMap().getTileIndex(position.x, position.y);
		if (tileIndex == map::TileIndex::INVALID)
		{
			luaL_error(L, "Trying to spawn an entity outside the map (%f, %f)", position.x, position.y);
		}
		position.z = baseMapState.getMap().getTileZ(tileIndex);
	}

	// heading
	float heading = static_cast<float>(luaL_optnumber(L, 3, 0.f));

	// elevation
	float elevation = static_cast<float>(luaL_optnumber(L, 4, 0.f));

	// components flags
	component::ComponentFlags componentFlags = static_cast<component::ComponentFlags>(luaL_optinteger(L, 5, component::AllComponents));
	luaL_argcheck(L, componentFlags != 0, 2, "componentFlags must not be zero");

	const std::shared_ptr<const EntityTemplate>& entityTemplate = baseMapState.getEntityTemplate(game, entityTemplateName);
	Entity* entity = baseMapState.spawnEntityAtPosition(game, entityTemplate, position, heading, elevation, componentFlags);
	pushEntity(L, entity);
	return 1;
}

int l_Entity_setGhostTemplate(lua_State* L)
{
	const char* ghostTemplateName = luaL_checkstring(L, 1);

	Game& game = flat::lua::getFlatAs<Game>(L);
	states::BaseMapState& baseMapState = game.getStateMachine().getState()->to<states::BaseMapState>();

	const std::shared_ptr<const EntityTemplate>& ghostTemplate = baseMapState.getEntityTemplate(game, ghostTemplateName);

	baseMapState.setGhostTemplate(game, ghostTemplate);
	return 0;
}

int l_Entity_clearGhostTemplate(lua_State* L)
{
	Game& game = flat::lua::getFlatAs<Game>(L);
	states::BaseMapState& baseMapState = game.getStateMachine().getState()->to<states::BaseMapState>();
	baseMapState.clearGhostTemplate();
	return 0;
}

EntityHandle getEntityHandle(lua_State* L, int index)
{
	return LuaEntityHandle::get(L, index);
}

// private
Entity& getEntity(lua_State* L, int index)
{
	EntityHandle entityHandle = getEntityHandle(L, index);
	Entity* entity = entityHandle.getEntity();
	if (entity == nullptr)
	{
		luaL_argerror(L, index, "The entity is not valid anymore");
	}
	return *entity;
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
		LuaEntityHandle::pushNew(L, entity);
	}
	else
	{
		lua_pushnil(L);
	}
}

} // lua
} // entity
} // game



