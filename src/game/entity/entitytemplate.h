#ifndef GAME_MAP_ENTITYTEMPLATE_H
#define GAME_MAP_ENTITYTEMPLATE_H

#include <memory>
#include <flat.h>
#include "sprite/description.h"
#include "component/componenttype.h"

namespace game
{
class Game;
namespace entity
{
namespace component
{
class ComponentRegistry;
class ComponentTemplate;
}
namespace behavior
{
class Behavior;
}

class EntityTemplate final
{
	public:
		EntityTemplate(Game& game, lua_State* L, const component::ComponentRegistry& componentRegistry, const std::string& path, const std::string& name);
		~EntityTemplate();

		inline component::ComponentFlags getComponentFlags() const { return m_componentFlags; }

		inline const std::string& getName() const { return m_name; }

		template <class ComponentType>
		inline const typename ComponentType::TemplateType* getComponentTemplate() const;
	
	private:
		std::string m_name;
		std::vector<std::unique_ptr<component::ComponentTemplate>> m_componentTemplates;
		component::ComponentFlags m_componentFlags;
};



template<class ComponentType>
inline const typename ComponentType::TemplateType* EntityTemplate::getComponentTemplate() const
{
	typename component::ComponentTemplate* componentTemplate = m_componentTemplates.at(ComponentType::getId() - 1).get();
	if (componentTemplate == nullptr)
	{
		return nullptr;
	}

	return componentTemplate->toP<typename ComponentType::TemplateType>();
}

} // entity
} // game

#endif // GAME_MAP_ENTITYTEMPLATE_H



