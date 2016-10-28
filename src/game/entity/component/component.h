#ifndef GAME_ENTITY_COMPONENT_COMPONENT_H
#define GAME_ENTITY_COMPONENT_COMPONENT_H

#include <cstdint>
#include <memory>
#include <flat.h>
#include "componenttype.h"
#include "componenttemplate.h"

namespace game
{
class Game;
namespace entity
{
class Entity;
class EntityTemplate;
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

		virtual const ComponentType& getComponentType() const = 0;

		static ComponentTemplate* loadConfigFile(Game& game, lua_State* L, const std::string& entityTemplatePath);

		const EntityTemplate& getEntityTemplate() const;
		
	protected:
		Entity* m_owner;
};

template <class ComponentTemplateType>
class ComponentImpl : public Component
{
	typedef Component Super;
	public:
		typedef ComponentTemplateType TemplateType;

	public:
		inline static const char* getConfigName() { FLAT_ASSERT_MSG(false, "ComponentTemplateType::getConfigName() missing"); return nullptr; }
		inline static void setType(const std::shared_ptr<const ComponentType>& type) { ComponentImpl::type = type; }
		inline static ComponentFlags getFlag() { return getType().getComponentTypeFlag(); }
		inline static ComponentTypeId getId() { return getType().getComponentTypeId(); }
		const ComponentType& getComponentType() const override final { FLAT_ASSERT_MSG(type != nullptr, "Component not registered"); return *type.get(); }

		static ComponentTemplate* loadConfigFile(Game& game, lua_State* L, const std::string& entityTemplatePath);

		template <class T = ComponentImpl<ComponentTemplateType>>
		inline const typename T::TemplateType* getTemplate() const;

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
	return getEntityTemplate().getComponentTemplate<T>();
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



