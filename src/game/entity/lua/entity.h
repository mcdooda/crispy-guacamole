#ifndef GAME_ENTITY_LUA_ENTITY_H
#define GAME_ENTITY_LUA_ENTITY_H

#include <flat.h>
#include "../entity.h"

namespace game
{
namespace entity
{
class Entity;
namespace lua
{

int open(Game& game);

int l_Entity_eq(lua_State* L);

int l_Entity_isValid(lua_State* L);
int l_Entity_delete(lua_State* L);

int l_Entity_getTemplateName(lua_State* L);
int l_Entity_hasComponent(lua_State* L);
int l_Entity_decComponentDisableLevel(lua_State* L);
int l_Entity_incComponentDisableLevel(lua_State* L);
int l_Entity_isComponentEnabled(lua_State* L);

#ifdef FLAT_DEBUG
int l_Entity_setDebug(lua_State* L);
int l_Entity_getDebug(lua_State* L);
int l_Entity_setComponentDebug(lua_State* L);
int l_Entity_getComponentDebug(lua_State* L);
#endif

int l_Entity_despawn(lua_State* L);

int l_Entity_getExtraData(lua_State* L);

int l_Entity_setPosition(lua_State* L);
int l_Entity_getPosition(lua_State* L);

int l_Entity_setHeading(lua_State* L);
int l_Entity_getHeading(lua_State* L);

int l_Entity_setElevation(lua_State* L);
int l_Entity_getElevation(lua_State* L);

int l_Entity_lookAtEntity(lua_State* L);

// ui
int l_Entity_setUiOffset(lua_State* L);
int l_Entity_setUiVisible(lua_State* L);

// movement
int l_Entity_moveTo(lua_State* L);
int l_Entity_clearPath(lua_State* L);
int l_Entity_setSpeed(lua_State* L);
int l_Entity_getSpeed(lua_State* L);
int l_Entity_jump(lua_State* L);
int l_Entity_restrictToZone(lua_State* L);

// behavior
int l_Entity_enterState(lua_State* L);
int l_Entity_sleep(lua_State* L);

// sprite
int l_Entity_playAnimation(lua_State* L);
int l_Entity_setMoveAnimation(lua_State* L);
int l_Entity_setDefaultMoveAnimation(lua_State* L);
int l_Entity_getAttachPoint(lua_State* L);

// detection
int l_Entity_canSee(lua_State* L);
int l_Entity_eachVisibleEntity(lua_State* L);

// faction
int l_Entity_isNeutral(lua_State* L);
int l_Entity_isFriendly(lua_State* L);
int l_Entity_isHostile(lua_State* L);

// attack
int l_Entity_setAttackTarget(lua_State* L);
int l_Entity_getAttackTarget(lua_State* L);
int l_Entity_isInAttackRange(lua_State* L);

// life
int l_Entity_isLiving(lua_State* L);
int l_Entity_kill(lua_State* L);
int l_Entity_dealDamage(lua_State* L);
int l_Entity_getHealth(lua_State* L);
int l_Entity_getMaxHealth(lua_State* L);
int l_Entity_healthChanged(lua_State* L);

// selection
int l_Entity_selected(lua_State* L);
int l_Entity_deselected(lua_State* L);
int l_Entity_click(lua_State* L);

// projectile
int l_Entity_setProjectileSpeed(lua_State* L);
int l_Entity_getProjectileSpeed(lua_State* L);

// static lua functions
int l_Entity_spawn(lua_State* L);

int l_Entity_setGhostTemplate(lua_State* L);
int l_Entity_clearGhostTemplate(lua_State* L);


// private
EntityHandle getEntityHandle(lua_State* L, int index);
Entity& getEntity(lua_State* L, int index);
Entity* getEntityPtr(lua_State* L, int index);
void pushEntity(lua_State* L, Entity* entity);

template <class T>
T& getComponent(lua_State* L, Entity& entity)
{
	T* component = entity.getComponent<T>();
	if (!component)
	{
		luaL_error(L, "%s has no %s component", entity.getTemplateName().c_str(), T::getConfigName());
	}
	return *component;
}

} // lua
} // entity
} // game

#endif // GAME_ENTITY_LUA_ENTITY_H



