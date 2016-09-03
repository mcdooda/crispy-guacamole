#ifndef GAME_ENTITY_COMPONENT_COMPONENT_H
#define GAME_ENTITY_COMPONENT_COMPONENT_H

#include <cstdint>

namespace game
{
namespace entity
{
class Entity;
namespace component
{

typedef uint32_t ComponentFlags;

class Component
{
	public:
		Component();
		virtual ~Component();
		
		virtual void setOwner(Entity* owner);
		inline Entity* getOwner() { return m_owner; }
		inline const Entity* getOwner() const { return m_owner; }
		
		virtual void update(float currentTime, float elapsedTime) = 0;
		
		virtual bool isBusy() const { return false; }

		virtual ComponentFlags getType() const = 0;
		
	protected:
		Entity* m_owner;
};

template <class ComponentType>
inline bool hasFlag(ComponentFlags flags)
{
	static_assert(std::is_base_of<component::Component, ComponentType>::value, "ComponentType must inherit from Component");
	return (flags & ComponentType::Type) == ComponentType::Type;
}

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_COMPONENT_H



