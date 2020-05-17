#include "entitytemplate.h"
#include "component/componentregistry.h"
#include "../game.h"

namespace game
{
namespace entity
{

EntityTemplate::EntityTemplate(const std::string& name, Game& game, const component::ComponentRegistry& componentRegistry, const std::filesystem::path& path) :
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

	std::filesystem::path configPath = m_path / (std::string(componentType.getConfigName()) + ".lua");
	int code = luaL_loadfile(L, configPath.string().c_str());
	if (code == LUA_OK)
	{
		code = lua_pcall(L, 0, 1, 0);
		if (code == LUA_OK)
		{
			if (lua_istable(L, -1))
			{
				componentTemplate = componentType.loadConfigFile(game, L, m_path);
				lua_pop(L, 1);
				return;
			}
			else
			{
				lua_pushstring(L, "table expected from component file");
			}
		}
	}
	else if (code == LUA_ERRFILE)
	{
		// LUA_ERRFILE -> component does not exist -> not an error -> pop error message and continue
		lua_pop(L, 1);
		return;
	}

	FLAT_LUA_IGNORE_ALL_STACK_GROWTH();
	luaL_error(L, "Error %s while loading component '%s' for entity '%s':\n%s",
		flat::lua::codeToString(code),
		componentType.getConfigName(),
		m_name.c_str(),
		luaL_checkstring(L, -1));
}

} // entity
} // game



