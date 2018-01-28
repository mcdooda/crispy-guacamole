#ifndef GAME_ENTITY_ENTITYTHREAD_H
#define GAME_ENTITY_ENTITYTHREAD_H

#include <flat.h>

namespace game
{
namespace entity
{
class Entity;

class EntityThread : public flat::lua::Thread
{
	using Super = flat::lua::Thread;
	public:
		EntityThread() = default;
		~EntityThread() = default;
		
		void start(lua_State* L, Entity* entity);
};

} // entity
} // game

#endif // GAME_ENTITY_ENTITYTHREAD_H


