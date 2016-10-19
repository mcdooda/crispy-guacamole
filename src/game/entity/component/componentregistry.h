#ifndef GAME_ENTITY_COMPONENT_COMPONENTREGISTRY_H
#define GAME_ENTITY_COMPONENT_COMPONENTREGISTRY_H

#include <vector>
#include <flat.h>
#include "component.h"
#include "componenttype.h"

namespace game
{
namespace entity
{
namespace component
{

class ComponentRegistry
{
public:
	ComponentRegistry();
	~ComponentRegistry();

	void registerComponentTypes();

	template <class T>
	inline void registerComponentType();

	inline size_t getNumComponentTypes() const { return m_componentTypes.size(); }

	template <class Func>
	void eachComponentType(Func func) const;

	// make ComponentRegistry& usable as a Resource constructor parameter
	bool operator<(const ComponentRegistry& other) const { return this < &other; }

private:
	std::vector<std::shared_ptr<ComponentType>> m_componentTypes;
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

template<class Func>
inline void ComponentRegistry::eachComponentType(Func func) const
{
	for (const std::shared_ptr<ComponentType>& componentType : m_componentTypes)
	{
		func(*componentType.get());
	}
}

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_COMPONENTREGISTRY_H


