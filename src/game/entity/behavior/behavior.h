#ifndef GAME_BEHAVIOR_H
#define GAME_BEHAVIOR_H

#include <flat.h>

namespace game
{
namespace entity
{
namespace behavior
{

class Behavior
{
	public:
		Behavior(lua_State* L, const std::string& fileName);
		
	private:
		void load(lua_State* L);
		
		std::string m_fileName;
		flat::lua::SharedReference<LUA_TTABLE> m_states;
};

} // behavior
} // entity
} // game

#endif // GAME_BEHAVIOR_H



