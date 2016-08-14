#ifndef GAME_ENTITY_COMPONENT_COMPONENT_H
#define GAME_ENTITY_COMPONENT_COMPONENT_H

namespace game
{
namespace entity
{
class Entity;
namespace component
{

class Component
{
	public:
		Component() : m_owner(nullptr) {}
		virtual ~Component() {}
		
		virtual void setOwner(Entity* owner);
		inline Entity* getOwner() { return m_owner; }
		inline const Entity* getOwner() const { return m_owner; }
		
		virtual void update(float currentTime, float elapsedTime) = 0;
		
		virtual bool isBusy() const { return false; }
		
	protected:
		Entity* m_owner;
};

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_COMPONENT_H



