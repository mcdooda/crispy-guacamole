#ifndef GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIOR_H
#define GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIOR_H

#include <flat.h>

namespace game::entity::component::behavior
{
class BehaviorRuntime;

class Behavior
{
	friend class BehaviorRuntime;
	public:
		Behavior(lua_State* L);
		~Behavior();
		
		void pushStates(lua_State* L) const;

	private:
		void load(lua_State* L);
		inline lua_State* getLuaState() const { return m_states.getLuaState(); }
		
	private:
		flat::lua::SharedLuaReference<LUA_TTABLE> m_states;
};

} // game::entity::component::behavior

#endif // GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIOR_H



