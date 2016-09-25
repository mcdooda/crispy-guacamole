#include "behavior.h"

namespace game
{
namespace entity
{
namespace behavior
{

Behavior::Behavior(lua_State* L, const std::string& fileName) :
	m_fileName(fileName)
{
	load(L);
}

Behavior::~Behavior()
{

}

void Behavior::load(lua_State* L)
{
	luaL_loadfile(L, m_fileName.c_str());
	lua_call(L, 0, 1);
	m_states.set(L, -1);
	lua_pop(L, 1);
}

void Behavior::pushStates(lua_State* L) const
{
	m_states.push(L);
}

} // behavior
} // entity
} // game



