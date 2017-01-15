#ifndef GAME_ENTITY_COMPONENT_COMPONENT_H
#define GAME_ENTITY_COMPONENT_COMPONENT_H

#include <cstdint>
#include <memory>
#include <flat.h>
#include "componenttype.h"
#include "componenttemplate.h"
#include "../entity.h"
#include "../entitytemplate.h"
#include "../../debug/debugdisplay.h"

namespace game
{
class Game;
namespace entity
{
class Entity;
namespace component
{

class Component : public flat::util::Convertible<Component>
{
	public:
		Component();
		virtual ~Component();
		
		inline void setOwner(Entity* owner) { m_owner = owner; }
		inline Entity* getOwner() { return m_owner; }
		inline const Entity* getOwner() const { return m_owner; }

		virtual void init() {}
		virtual void update(float currentTime, float elapsedTime) = 0;
		virtual bool isBusy() const { return false; }

		FLAT_DEBUG_ONLY(virtual void debugDraw(debug::DebugDisplay& debugDisplay) const {})

		virtual const ComponentType& getComponentType() const = 0;

		static ComponentTemplate* loadConfigFile(Game& game, lua_State* L, const std::string& entityTemplatePath);

		const EntityTemplate& getEntityTemplate() const;

		inline void enable() { m_enabled = true; }
		inline void disable() { m_enabled = false; }
		inline bool isEnabled() const { return m_enabled; }
		
	protected:
		Entity* m_owner;

	private:
		bool m_enabled : 1;
		FLAT_DEBUG_ONLY(bool m_debug : 1;)
};

template <class ComponentTemplateType>
class ComponentImpl : public Component
{
	typedef Component Super;
	public:
		typedef ComponentTemplateType TemplateType;

	public:
		inline static const char* getConfigName() { FLAT_ASSERT_MSG(false, "ComponentTemplateType::getConfigName() missing"); return nullptr; }
		inline static bool enableInEditor() { return true; }
		inline static bool allowEntityInEditor() { return true; }
		inline static void setType(const std::shared_ptr<const ComponentType>& type) { ComponentImpl::type = type; }
		inline static ComponentFlags getFlag() { return getType().getComponentTypeFlag(); }
		inline static ComponentTypeId getId() { return getType().getComponentTypeId(); }
		const ComponentType& getComponentType() const override final { FLAT_ASSERT_MSG(type != nullptr, "Component not registered"); return *type.get(); }

		static ComponentTemplate* loadConfigFile(Game& game, lua_State* L, const std::string& entityTemplatePath);

		template <class T = ComponentImpl<ComponentTemplateType>>
		inline const typename T::TemplateType* getTemplate() const;
		
		template <class T>
		inline void enableComponent() const;
		
		template <class T>
		inline void disableComponent() const;

	private:
		inline static const ComponentType& getType();

		static std::shared_ptr<const ComponentType> type;
};

template <class ComponentTemplateType>
std::shared_ptr<const ComponentType> ComponentImpl<ComponentTemplateType>::type;

template <class ComponentTemplateType>
inline ComponentTemplate* ComponentImpl<ComponentTemplateType>::loadConfigFile(Game& game, lua_State* L, const std::string& entityTemplatePath)
{
	ComponentTemplateType* componentTemplate = new ComponentTemplateType();
	componentTemplate->load(game, L, entityTemplatePath);
	return componentTemplate;
}

template<class ComponentTemplateType>
template<class T>
inline const typename T::TemplateType* ComponentImpl<ComponentTemplateType>::getTemplate() const
{
	const EntityTemplate& entityTemplate = getEntityTemplate();
	return entityTemplate.getComponentTemplate<T>();
}

template <class ComponentTemplateType>
template <class T>
inline void ComponentImpl<ComponentTemplateType>::enableComponent() const
{
	static_assert(!std::is_same<ComponentTemplateType, T>::value, "call enable() instead of T::enableComponent<T>()");
	T* otherComponent = m_owner->getComponent<T>();
	if (otherComponent)
	{
		FLAT_ASSERT(!otherComponent->isEnabled());
		otherComponent->enable();
	}
}

template <class ComponentTemplateType>
template <class T>
inline void ComponentImpl<ComponentTemplateType>::disableComponent() const
{
	static_assert(!std::is_same<ComponentTemplateType, T>::value, "call disable() instead of T::disableComponent<T>()");
	T* otherComponent = m_owner->getComponent<T>();
	if (otherComponent)
	{
		FLAT_ASSERT(otherComponent->isEnabled());
		otherComponent->disable();
	}
}

template<class ComponentTemplateType>
inline const ComponentType& ComponentImpl<ComponentTemplateType>::getType()
{
	FLAT_ASSERT_MSG(type != nullptr, "Component not registered");
	return *type.get();
}

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_COMPONENT_H



