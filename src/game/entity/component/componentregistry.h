#ifndef GAME_ENTITY_COMPONENT_COMPONENTREGISTRY_H
#define GAME_ENTITY_COMPONENT_COMPONENTREGISTRY_H

#include <vector>
#include <functional>
#include <flat.h>
#include "component.h"

namespace game
{
namespace entity
{
namespace component
{
class ComponentType;

class ComponentRegistry
{
public:
	ComponentRegistry();
	~ComponentRegistry();

	void registerComponentTypes();

	template <class T>
	inline void registerComponentType();

	inline size_t getNumComponentTypes() { return m_componentTypes.size(); }
	inline ComponentType* getComponentType(int index) { return m_componentTypes[index].get(); }

	void eachComponentType(std::function<void(const ComponentType*)> func);

private:
	std::vector<std::shared_ptr<ComponentType>> m_componentTypes;
};

class ComponentType
{
public:
	ComponentType(ComponentTypeId typeId) : m_typeId(typeId) {}

	inline ComponentTypeId getComponentTypeId() const { return m_typeId; }
	inline ComponentFlags getComponentTypeFlag() const;
	virtual size_t getComponentSize() const = 0;

	virtual Component* createComponent(flat::containers::DynamicPool* pool) const = 0;
	virtual void destroyComponent(flat::containers::DynamicPool* pool, Component* component) const = 0;
private:
	ComponentTypeId m_typeId;
};

ComponentFlags ComponentType::getComponentTypeFlag() const
{
	ComponentFlags flags = 1 << getComponentTypeId();
	FLAT_ASSERT(flags != 0);
	return flags;
}

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

template<class T>
inline void ComponentRegistry::registerComponentType()
{
	size_t id = m_componentTypes.size() + 1;
	FLAT_ASSERT(id <= std::numeric_limits<ComponentTypeId>::max());
	std::shared_ptr<ComponentType> componentType = std::make_shared<ComponentTypeImpl<T>>(static_cast<ComponentTypeId>(id));
	T::setType(componentType);
	m_componentTypes.push_back(componentType);
}

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

#endif // GAME_ENTITY_COMPONENT_COMPONENTREGISTRY_H


