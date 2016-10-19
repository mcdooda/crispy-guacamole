#ifndef GAME_ENTITY_BEHAVIOR_BEHAVIOR_H
#define GAME_ENTITY_BEHAVIOR_BEHAVIOR_H

#include <flat.h>

namespace game
{
namespace entity
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
		void pushStates(lua_State*) const;
		inline lua_State* getLuaState() const { return m_states.getLuaState(); }
		
	private:
		flat::lua::SharedLuaReference<LUA_TTABLE> m_states;
};

} // behavior
} // entity
} // game

#endif // GAME_ENTITY_BEHAVIOR_BEHAVIOR_H



