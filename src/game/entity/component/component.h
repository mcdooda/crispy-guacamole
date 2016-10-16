#ifndef GAME_ENTITY_COMPONENT_COMPONENT_H
#define GAME_ENTITY_COMPONENT_COMPONENT_H

#include <cstdint>
#include <memory>
#include <flat.h>

namespace game
{
class Game;
namespace entity
{
class Entity;
namespace component
{
class ComponentType;
class ComponentTemplate;

typedef uint8_t ComponentTypeId;

typedef uint32_t ComponentFlags;
enum { AllComponents = 0xFFFFFFFF };

class Component
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

		static std::shared_ptr<ComponentTemplate> loadConfigFile(Game& game, lua_State* L, const std::string& entityTemplatePath);
		
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
		inline static ComponentFlags getFlag() { return type.get()->getComponentTypeFlag(); }
		inline static ComponentTypeId getId() { return type.get()->getComponentTypeId(); }
		const ComponentType& getComponentType() const override { return *type.get(); }

		static std::shared_ptr<ComponentTemplate> loadConfigFile(Game& game, lua_State* L, const std::string& entityTemplatePath);

	private:
		static std::shared_ptr<const ComponentType> type;
};

template <class ComponentTemplateType>
std::shared_ptr<const ComponentType> ComponentImpl<ComponentTemplateType>::type;

template<class ComponentTemplateType>
inline std::shared_ptr<ComponentTemplate> ComponentImpl<ComponentTemplateType>::loadConfigFile(Game& game, lua_State* L, const std::string& entityTemplatePath)
{
	ComponentTemplateType* componentTemplate = new ComponentTemplateType();
	componentTemplate->load(game, L, entityTemplatePath);
	return std::shared_ptr<ComponentTemplate>(componentTemplate);
}

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_COMPONENT_H



