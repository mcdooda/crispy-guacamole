#ifndef GAME_ENTITY_COMPONENT_COMPONENTTYPE_H
#define GAME_ENTITY_COMPONENT_COMPONENTTYPE_H

#include <flat.h>

namespace game
{
class Game;
namespace entity
{
namespace component
{
class Component;
class ComponentTemplate;

typedef uint8_t ComponentTypeId;

typedef uint32_t ComponentFlags;
enum { AllComponents = 0xFFFFFFFF };

class ComponentType
{
public:
	ComponentType(ComponentTypeId typeId) : m_typeId(typeId) {}

	inline ComponentTypeId getComponentTypeId() const { return m_typeId; }
	ComponentFlags getComponentTypeFlag() const;

	virtual size_t getComponentSize() const = 0;
	virtual const char* getConfigName() const = 0;
	virtual ComponentTemplate* loadConfigFile(Game& game, lua_State* L, const std::string& entityTemplatePath) const = 0;

	virtual Component* createComponent(flat::containers::DynamicPool& pool) const = 0;
	virtual void destroyComponent(flat::containers::DynamicPool& pool, Component* component) const = 0;
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
	const char* getConfigName() const override;
	ComponentTemplate* loadConfigFile(Game& game, lua_State* L, const std::string& entityTemplatePath) const override;

	Component* createComponent(flat::containers::DynamicPool& pool) const override;
	void destroyComponent(flat::containers::DynamicPool& pool, Component* component) const override;
};

template <class T>
size_t ComponentTypeImpl<T>::getComponentSize() const
{
	return sizeof(T);
}

template <class T>
const char* ComponentTypeImpl<T>::getConfigName() const
{
	return T::getConfigName();
}

template <class T>
ComponentTemplate* ComponentTypeImpl<T>::loadConfigFile(Game& game, lua_State* L, const std::string& entityTemplatePath) const
{
	return T::loadConfigFile(game, L, entityTemplatePath);
}

template <class T>
Component* ComponentTypeImpl<T>::createComponent(flat::containers::DynamicPool& pool) const
{
	return pool.create<T>();
}

template <class T>
void ComponentTypeImpl<T>::destroyComponent(flat::containers::DynamicPool& pool, Component* component) const
{
	FLAT_ASSERT(dynamic_cast<T*>(component) != nullptr);
	pool.destroy<T>(static_cast<T*>(component));
}

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_COMPONENTTYPE_H


