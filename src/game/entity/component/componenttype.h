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

using ComponentTypeId = uint8_t;

using ComponentFlags = uint32_t;
enum { AllComponents = 0xFFFFFFFF };

enum ComponentUpdateType : uint8_t
{
	NONE = 0,
	PRE_UPDATE = (1 << 0),
	UPDATE = (1 << 1),
	POST_UPDATE = (1 << 2)
};

class ComponentType
{
public:
	ComponentType(ComponentTypeId typeId) : m_typeId(typeId) {}

	inline ComponentTypeId getComponentTypeId() const { return m_typeId; }
	ComponentFlags getComponentTypeFlag() const;

	virtual size_t getComponentSize() const = 0;

	virtual const char* getConfigName() const = 0;
	virtual const char* getVisualName() const = 0;
	virtual bool enableInMapEditor() const = 0;
	virtual bool allowEntityInEditor() const = 0;

	virtual ComponentUpdateType getUpdateType() const = 0;
	virtual int getUpdatePeriod() const = 0;

	virtual ComponentTemplate* loadConfigFile(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath) const = 0;

	virtual Component* createComponent(flat::containers::DynamicPool& pool) const = 0;
	virtual void destroyComponent(flat::containers::DynamicPool& pool, Component* component) const = 0;
private:
	ComponentTypeId m_typeId;
};

template <class T>
class ComponentTypeImpl : public ComponentType
{
	using Super = ComponentType;
public:
	ComponentTypeImpl(ComponentTypeId typeId) : Super(typeId) {}

	size_t getComponentSize() const override;

	const char* getConfigName() const override;
	const char* getVisualName() const override;
	bool enableInMapEditor() const override;
	bool allowEntityInEditor() const override;

	ComponentUpdateType getUpdateType() const override;
	int getUpdatePeriod() const override;

	ComponentTemplate* loadConfigFile(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath) const override;

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
const char* ComponentTypeImpl<T>::getVisualName() const
{
	return T::getVisualName();
}

template <class T>
bool ComponentTypeImpl<T>::enableInMapEditor() const
{
	return T::enableInMapEditor();
}

template <class T>
bool ComponentTypeImpl<T>::allowEntityInEditor() const
{
	return T::allowEntityInEditor();
}

template<class T>
inline ComponentUpdateType ComponentTypeImpl<T>::getUpdateType() const
{
	return T::getUpdateType();
}

template<class T>
inline int ComponentTypeImpl<T>::getUpdatePeriod() const
{
	return T::getUpdatePeriod();
}

template <class T>
ComponentTemplate* ComponentTypeImpl<T>::loadConfigFile(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath) const
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


