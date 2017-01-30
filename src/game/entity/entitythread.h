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
	typedef flat::lua::Thread Super;
	public:
		EntityThread() = default;
		~EntityThread() = default;
		
		void start(Entity* entity);
};

} // entity
} // game

#endif // GAME_ENTITY_ENTITYTHREAD_H


