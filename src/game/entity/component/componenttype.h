#ifndef GAME_ENTITY_COMPONENT_COMPONENTTYPE_H
#define GAME_ENTITY_COMPONENT_COMPONENTTYPE_H

#include <flat.h>
#include "component.h"

namespace game
{
namespace entity
{
namespace component
{

class ComponentType
{
public:
	ComponentType(ComponentTypeId typeId) : m_typeId(typeId) {}

	inline ComponentTypeId getComponentTypeId() const { return m_typeId; }
	ComponentFlags getComponentTypeFlag() const;
	virtual size_t getComponentSize() const = 0;

	virtual Component* createComponent(flat::containers::DynamicPool* pool) const = 0;
	virtual void destroyComponent(flat::containers::DynamicPool* pool, Component* component) const = 0;
private:
	ComponentTypeId m_typeId;
};

template <class T>
class ComponentTypeImpl : public ComponentType
{
	typedef ComponentType Super;
public:
	ComponentTypeImpl(ComponentTypeId typeId) : Super(typeId) {}

	size_t getComponentSize() const override;
	Component* createComponent(flat::containers::DynamicPool* pool) const override;
	void destroyComponent(flat::containers::DynamicPool* pool, Component* component) const override;
};

template <class T>
inline size_t ComponentTypeImpl<T>::getComponentSize() const
{
	return sizeof(T);
}

template <class T>
Component* ComponentTypeImpl<T>::createComponent(flat::containers::DynamicPool* pool) const
{
	return pool->create<T>();
}

template <class T>
void ComponentTypeImpl<T>::destroyComponent(flat::containers::DynamicPool* pool, Component* component) const
{
	FLAT_ASSERT(dynamic_cast<T*>(component) != nullptr);
	pool->destroy<T>(static_cast<T*>(component));
}

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_COMPONENTTYPE_H


