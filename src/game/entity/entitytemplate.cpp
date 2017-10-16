#include "entitytemplate.h"
#include "component/componentregistry.h"
#include "../game.h"

namespace game
{
namespace entity
{

EntityTemplate::EntityTemplate(Game& game, const component::ComponentRegistry& componentRegistry, const std::string& path, const std::string& name) :
	m_name(name),
	m_componentFlags(0)
{
	lua_State* L = game.lua->state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		m_componentTemplates.resize(componentRegistry.getNumComponentTypes());
		componentRegistry.eachComponentType([this, &game, L, &path, &name](const component::ComponentType& componentType)
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

			std::string configPath = path + componentType.getConfigName() + ".lua";
			int code = luaL_loadfile(L, configPath.c_str());
			if (code == LUA_OK)
			{
				code = lua_pcall(L, 0, 1, 0);
				if (code == LUA_OK)
				{
					component::ComponentTemplate* componentTemplate = componentType.loadConfigFile(game, L, path);
					if (componentTemplate)
					{
						int index = componentType.getComponentTypeId() - 1;
						m_componentTemplates[index].reset(componentTemplate);
						m_componentFlags |= componentType.getComponentTypeFlag();
					}
				}
				else
				{
					std::cerr << "Error while loading " << componentType.getConfigName() << " component on " << name << ":" << std::endl
						<< luaL_checkstring(L, -1) << std::endl << std::endl;
				}
				lua_pop(L, 1); // pop config table
			}
			else if (code == LUA_ERRFILE)
			{
				// LUA_ERRFILE -> component does not exist -> not an error -> pop error message and continue
				lua_pop(L, 1);
			}
			else
			{
				luaL_error(L, "luaL_loadfile failed with error %s", flat::lua::codeToString(code));
			}
		});
	}
}

EntityTemplate::~EntityTemplate()
{
	
}

} // entity
} // game



