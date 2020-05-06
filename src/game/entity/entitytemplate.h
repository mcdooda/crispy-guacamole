#ifndef GAME_ENTITY_ENTITYTEMPLATE_H
#define GAME_ENTITY_ENTITYTEMPLATE_H

#include <memory>
#include <flat.h>
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

class EntityTemplate final
{
	public:
		EntityTemplate(const std::string& path, Game& game, const component::ComponentRegistry& componentRegistry);
		~EntityTemplate();

		inline const std::string& getPath() const { return m_path; }

		inline component::ComponentFlags getComponentFlags() const { return m_componentFlags; }

		template <class ComponentType>
		inline const typename ComponentType::TemplateType* getComponentTemplate() const;

#ifdef FLAT_DEBUG
		void reloadComponent(Game& game, const component::ComponentRegistry& componentRegistry, component::ComponentFlags componentFlag, bool addComponent);
		void removeComponent(Game& game, const component::ComponentRegistry& componentRegistry, component::ComponentFlags componentFlag);
#endif

	private:
		component::ComponentTemplate* loadComponentTemplate(Game& game, const component::ComponentType& componentType) const;
		void loadComponentTemplateSafe(lua_State* L, Game& game, const component::ComponentType& componentType, component::ComponentTemplate*& componentTemplate) const;
	
	private:
		std::string m_path;
		std::vector<std::unique_ptr<component::ComponentTemplate>> m_componentTemplates;
		component::ComponentFlags m_componentFlags;
};



template<class ComponentType>
inline const typename ComponentType::TemplateType* EntityTemplate::getComponentTemplate() const
{
	const std::uint8_t componentTemplateIndex = ComponentType::getId() - 1;
	typename component::ComponentTemplate* componentTemplate = m_componentTemplates.at(componentTemplateIndex).get();
	if (componentTemplate == nullptr)
	{
		return nullptr;
	}

	FLAT_ASSERT(dynamic_cast<typename ComponentType::TemplateType*>(componentTemplate) != nullptr);
	return static_cast<typename ComponentType::TemplateType*>(componentTemplate);
}

} // entity
} // game

#endif // GAME_ENTITY_ENTITYTEMPLATE_H



