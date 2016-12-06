#ifndef GAME_ENTITY_ENTITYTHREAD_H
#define GAME_ENTITY_ENTITYTHREAD_H

#include <flat.h>

namespace game
{
namespace entity
{
class Entity;

class EntityThread final
{
	public:
		EntityThread();
		~EntityThread();

		void set(lua_State* L, int index);

		void start(Entity* entity);
		void update(Entity* entity);

		inline bool isRunning() const { return m_status == LUA_YIELD; }
		inline bool isFinished() const { return m_status == LUA_OK; }

	protected:
		lua_State* m_mainThread;
		flat::lua::SharedLuaReference<LUA_TFUNCTION> m_function;
		flat::lua::SharedLuaReference<LUA_TTHREAD> m_thread;
		int m_status;
};

} // entity
} // game

#endif // GAME_ENTITY_ENTITYTHREAD_H


