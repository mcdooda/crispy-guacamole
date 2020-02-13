#include "entitytemplate.h"
#include "component/componentregistry.h"
#include "../game.h"

namespace game
{
namespace entity
{

EntityTemplate::EntityTemplate(const std::string& name, Game& game, const component::ComponentRegistry& componentRegistry, const std::string& path) :
	m_path(path),
	m_name(name),
	m_componentFlags(0)
{
	lua_State* L = game.lua->state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		m_componentTemplates.resize(componentRegistry.getNumComponentTypes());
		componentRegistry.eachComponentType([this, &game, L](const component::ComponentType& componentType)
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

			component::ComponentTemplate* componentTemplate = loadComponentTemplate(game, componentType);
			if (componentTemplate != nullptr)
			{
				int index = componentType.getComponentTypeId() - 1;
				m_componentTemplates[index].reset(componentTemplate);
				m_componentFlags |= componentType.getComponentTypeFlag();
			}
		});
	}
}

EntityTemplate::~EntityTemplate()
{
	
}

#ifdef FLAT_DEBUG
void EntityTemplate::reloadComponent(Game& game, const component::ComponentRegistry& componentRegistry, component::ComponentFlags componentFlag, bool addComponent)
{
	const component::ComponentType& componentType = componentRegistry.getComponentType(componentFlag);
	FLAT_ASSERT(addComponent || (m_componentFlags & componentType.getComponentTypeFlag()) != 0);

	component::ComponentTemplate* componentTemplate = loadComponentTemplate(game, componentType);
	if (componentTemplate != nullptr)
	{
		int index = componentType.getComponentTypeId() - 1;
		FLAT_ASSERT(addComponent != (m_componentTemplates[index] != nullptr));
		m_componentTemplates[index].reset(componentTemplate);

		if (addComponent)
		{
			m_componentFlags |= componentType.getComponentTypeFlag();
		}
	}

}

void EntityTemplate::removeComponent(Game& game, const component::ComponentRegistry& componentRegistry, component::ComponentFlags componentFlag)
{
	const component::ComponentType& componentType = componentRegistry.getComponentType(componentFlag);
	FLAT_ASSERT((m_componentFlags & componentType.getComponentTypeFlag()) != 0);

	int index = componentType.getComponentTypeId() - 1;
	FLAT_ASSERT(m_componentTemplates[index] != nullptr);
	m_componentTemplates[index].reset();
	m_componentFlags &= ~componentType.getComponentTypeFlag();
}
#endif

component::ComponentTemplate* EntityTemplate::loadComponentTemplate(Game& game, const component::ComponentType& componentType) const
{
	component::ComponentTemplate* componentTemplate = nullptr;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(game.lua->state, 0);
		game.lua->protectedCall(this, &EntityTemplate::loadComponentTemplateSafe, game, componentType, componentTemplate);
	}
	return componentTemplate;
}

void EntityTemplate::loadComponentTemplateSafe(lua_State* L, Game& game, const component::ComponentType& componentType, component::ComponentTemplate*& componentTemplate) const
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	std::string configPath = m_path + componentType.getConfigName() + ".lua";
	int code = luaL_loadfile(L, configPath.c_str());
	if (code == LUA_OK)
	{
		code = lua_pcall(L, 0, 1, 0);
		if (code == LUA_OK)
		{
			componentTemplate = componentType.loadConfigFile(game, L, m_path);
			lua_pop(L, 1); // pop config table
			return;
		}
		else
		{
			std::cerr << "Error while loading " << componentType.getConfigName() << " component on " << m_name << ":" << std::endl
				<< luaL_checkstring(L, -1) << std::endl << std::endl;
			lua_pop(L, 1); // pop config table
		}
	}
	else if (code == LUA_ERRFILE)
	{
		// LUA_ERRFILE -> component does not exist -> not an error -> pop error message and continue
		lua_pop(L, 1);
	}
	else
	{
		flat::lua::printStack(L);
		luaL_error(L, "luaL_loadfile failed with error %s", flat::lua::codeToString(code));
	}
}

} // entity
} // game



