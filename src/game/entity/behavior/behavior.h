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
		Behavior(lua_State* L, const std::string& fileName);
		
	private:
		void load(lua_State* L);
		lua_State* pushStates() const;
		inline lua_State* getLuaState() const { return m_states.getLuaState(); }
		
	private:
		std::string m_fileName;
		flat::lua::SharedReference<LUA_TTABLE> m_states;
};

} // behavior
} // entity
} // game

#endif // GAME_ENTITY_BEHAVIOR_BEHAVIOR_H



