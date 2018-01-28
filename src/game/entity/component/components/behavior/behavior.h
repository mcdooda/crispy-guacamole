#ifndef GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIOR_H
#define GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIOR_H

#include <flat.h>

namespace game
{
namespace entity
{
namespace component
{
namespace behavior
{
class BehaviorRuntime;

class Behavior
{
	friend class BehaviorRuntime;
	public:
		Behavior(lua_State* L);
		~Behavior();
		
	private:
		void load(lua_State* L);
		void pushStates(lua_State* L) const;
		lua_State* getLuaState() const { FLAT_ASSERT(false); return nullptr; }
		
	private:
		flat::lua::SharedLuaReference<LUA_TTABLE> m_states;
};

} // behavior
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIOR_H



